#include "HttpServer.h"
#include "VirtualJoystick.h"
#include <QHttpServer>
#include <QNetworkInterface>
#include <QFile>

using StatusCode = QHttpServerResponder::StatusCode;


///////////////////////////////////////////////////////////////////////////////
//  CONSTRUCTEUR ET DESTRUCTEUR
//  ETHERNET LOCAL IP ADDRESS
//  URL
//
//  LISTEN
//  REGISTER REMOTE CONTROLLER
//  REGISTER VIRTUAL JOYSTICK
//  CLOSE SERVER
//
//  PROCESS FILE
//  PROCESS KEY
//  PROCESS BUTTON
//  PROCESS AXIS
//  PROCESS POV
///////////////////////////////////////////////////////////////////////////////


// CONSTRUCTEUR ET DESTRUCTEUR ////////////////////////////////////////////////
HttpServer::HttpServer()
{
	m_httpServer = nullptr;
}

HttpServer::~HttpServer()
{
	this->closeServer();
}

// ETHERNET LOCAL IP ADDRESS //////////////////////////////////////////////////
QString HttpServer::ethernetLocalIpAddress(bool ipv6)
{
	// search for the first active ethernet network interface
	QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
	auto isActiveEthernet = [] (const QNetworkInterface &ni) {return ni.type() == QNetworkInterface::Ethernet && (ni.flags() & QNetworkInterface::IsUp);};
	auto result1 = std::find_if(interfaces.begin(),interfaces.end(),isActiveEthernet);
	if (result1 == interfaces.end()) {return QString{};}
	
	// search for the first ip address with the right protocol
	QList<QNetworkAddressEntry> addressesEntries = result1->addressEntries();
	QAbstractSocket::NetworkLayerProtocol protocolToSearch = (ipv6 ? QAbstractSocket::IPv6Protocol : QAbstractSocket::IPv4Protocol);
	auto isIpVX = [protocolToSearch] (const QNetworkAddressEntry &nae) {return nae.ip().protocol() == protocolToSearch;};
	auto result2 = std::find_if(addressesEntries.begin(),addressesEntries.end(),isIpVX);
	if (result2 == addressesEntries.end()) {return QString{};}
	return result2->ip().toString();
}

// URL ////////////////////////////////////////////////////////////////////////
QString HttpServer::url(const QString &name) const
{
	if (!m_resourcesDir.contains(name)) {return {};}
	QString out = "http://" + HttpServer::ethernetLocalIpAddress() + ":8080/file/" + name + "/gui.html";
	return out;
}






// LISTEN /////////////////////////////////////////////////////////////////////
bool HttpServer::listen()
{
	if (!m_httpServer) {return false;}
	
	// routing for files (typically html, css, js, images, ...)
	m_httpServer->route("/file/<arg>/<arg>", [this] (const QString &name, const QString &fileName) {
		return this->processFile(name,fileName);
	});
	
	// routing for inputs
	m_httpServer->route("/key/<arg>/<arg>", [this] (const QString &keyStr, uint bDown) {
		return this->processKey(keyStr,bDown!=0);
	});
	m_httpServer->route("/button/<arg>/<arg>/<arg>", [this] (uint vj, uint button, uint bPressed) {
		return this->processButton(vj,button,bPressed!=0);
	});
	m_httpServer->route("/axis/<arg>/<arg>/<arg>", [this] (uint vj, uint axis, float value) {
		return this->processAxis(vj,axis,value);
	});
	m_httpServer->route("/pov/<arg>/<arg>/<arg>", [this] (uint vj, uint pov, float value) {
		return this->processPov(vj,pov,value);
	});
	
	return m_httpServer->listen(QHostAddress::Any,8080);
}

// REGISTER REMOTE CONTROLLER /////////////////////////////////////////////////
bool HttpServer::registerRemoteController(const QString &name, const QString &dirPath, QString &errorMessage)
{
	if (name.contains('/'))
	{
		errorMessage = "The name must not contain '/'";
		return false;
	}
	if (m_resourcesDir.contains(name))
	{
		errorMessage = "This name is already registered";
		return false;
	}
	if (!m_httpServer) {m_httpServer = new QHttpServer{};}
	
	m_resourcesDir.insert(name,dirPath);
	return true;
}

// REGISTER VIRTUAL JOYSTICK //////////////////////////////////////////////////
bool HttpServer::registerVirtualJoystick(uint id, QString &errorMessage)
{
	try
	{
		m_virtualJoysticks.push_back(std::make_unique<VirtualJoystick>(id)); // might throw
		errorMessage = "";
		return true;
	}
	catch (std::exception &e)
	{
		errorMessage = e.what();
		return false;
	}
}

// CLOSE SERVER ///////////////////////////////////////////////////////////////
void HttpServer::closeServer()
{
	if (m_httpServer)
	{
		// the destruction of the QHttpServer close the connections...
		// QHttpServer does not provide a "close" function!
		delete m_httpServer;
		m_httpServer = nullptr;
		m_resourcesDir.clear();
	}
}






// PROCESS FILE ///////////////////////////////////////////////////////////////
QHttpServerResponse HttpServer::processFile(const QString &name, const QString &fileName)
{
	if (!m_resourcesDir.contains(name)) {return QHttpServerResponse(StatusCode::BadRequest);}
	
	if (fileName == "gui.html")
	{
		QFile file{m_resourcesDir[name] + "/gui.html"};
		if (!file.open(QIODevice::ReadOnly)) {return QHttpServerResponse{StatusCode::NotFound};}
		QString html = file.readAll();
		html.replace("@name@",name);
		return QHttpServerResponse("text/html",html.toUtf8(),StatusCode::Ok);
	}
	else
	{
		return QHttpServerResponse::fromFile(m_resourcesDir[name] + "/" + fileName);
	}
}

// PROCESS KEY ////////////////////////////////////////////////////////////////
QHttpServerResponse HttpServer::processKey(const QString &keyStr, bool bDown)
{
	if (!keyStrokeGenerator.generateKeyStroke(keyStr,bDown))
		return QHttpServerResponse{"text/plain","Incorrect key name",StatusCode::PreconditionFailed};
	
	return QHttpServerResponse(StatusCode::NoContent);
}

// PROCESS BUTTON /////////////////////////////////////////////////////////////
QHttpServerResponse HttpServer::processButton(uint vji, uint button, bool bPressed)
{
	auto f = [vji] (const std::unique_ptr<VirtualJoystick> &up) {return up->id() == vji;};
	auto result = std::find_if(m_virtualJoysticks.begin(),m_virtualJoysticks.end(),f);
	if (result == m_virtualJoysticks.end())
		return QHttpServerResponse{"text/plain","Virtual joystick not acquired",StatusCode::PreconditionFailed};
	
	VirtualJoystick *vj = result->get();
	if (!vj->setButton(button,bPressed))
		return QHttpServerResponse{"text/plain","Button number is too big",StatusCode::Forbidden};
	
	vj->flush();
	return QHttpServerResponse(StatusCode::NoContent);
}

// PROCESS AXIS ///////////////////////////////////////////////////////////////
QHttpServerResponse HttpServer::processAxis(uint vji, uint axis, float value)
{
	auto f = [vji] (const std::unique_ptr<VirtualJoystick> &up) {return up->id() == vji;};
	auto result = std::find_if(m_virtualJoysticks.begin(),m_virtualJoysticks.end(),f);
	if (result == m_virtualJoysticks.end())
		return QHttpServerResponse{"text/plain","Virtual joystick not acquired",StatusCode::PreconditionFailed};
	
	VirtualJoystick *vj = result->get();
	if (!vj->setAxis(axis,value))
		return QHttpServerResponse{"text/plain","Axis number is too big",StatusCode::Forbidden};
	
	vj->flush();
	return QHttpServerResponse(StatusCode::NoContent);
}

// PROCESS POV ////////////////////////////////////////////////////////////////
QHttpServerResponse HttpServer::processPov(uint vji, uint pov, float value)
{
	auto f = [vji] (const std::unique_ptr<VirtualJoystick> &up) {return up->id() == vji;};
	auto result = std::find_if(m_virtualJoysticks.begin(),m_virtualJoysticks.end(),f);
	if (result == m_virtualJoysticks.end())
		return QHttpServerResponse{"text/plain","Virtual joystick not acquired",StatusCode::PreconditionFailed};
	
	VirtualJoystick *vj = result->get();
	if (!vj->setPov(pov,value))
		return QHttpServerResponse{"text/plain","Pov number is too big",StatusCode::Forbidden};
	
	vj->flush();
	return QHttpServerResponse(StatusCode::NoContent);
}

