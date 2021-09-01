#include "MyMfdHttpServer.h"
#include "VirtualJoystick.h"
#include <QNetworkInterface>
#include <QFile>


///////////////////////////////////////////////////////////////////////////////
//  CONSTRUCTEUR
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


// CONSTRUCTEUR ///////////////////////////////////////////////////////////////
MyMfdHttpServer::MyMfdHttpServer(const HttpServerConfig &config) : HttpServer{config}
{
	// routing for files (typically html, css, js, images, ...)
	this->addRoute("GET", "^/file/(\\w|.|-)+/(\\w|.|-)+$", this, &MyMfdHttpServer::processFile);

	// routing for inputs
	this->addRoute("GET", "^/key/\\w+(\\+\\w+)?/\\d$", this, &MyMfdHttpServer::processKey);
	this->addRoute("GET", "^/button/\\d+/\\d+/\\d$", this, &MyMfdHttpServer::processButton);
	this->addRoute("GET", "^/axis/\\d+/\\d+/-?\\d(.\\d+)?$", this, &MyMfdHttpServer::processAxis);
	this->addRoute("GET", "^/pov/\\d+/\\d+/-?\\d(.\\d+)?$", this, &MyMfdHttpServer::processPov);
}

// ETHERNET LOCAL IP ADDRESS //////////////////////////////////////////////////
QString MyMfdHttpServer::ethernetLocalIpAddress(bool ipv6)
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
QString MyMfdHttpServer::url(const QString &name) const
{
	if (!m_resourcesDir.contains(name)) {return {};}
	QString out = "http://" + MyMfdHttpServer::ethernetLocalIpAddress() + ":8080/file/" + name + "/gui.html";
	return out;
}






// REGISTER REMOTE CONTROLLER /////////////////////////////////////////////////
bool MyMfdHttpServer::registerRemoteController(const QString &name, const QString &dirPath, QString &errorMessage)
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
	
	m_resourcesDir.insert(name,dirPath);
	return true;
}

// REGISTER VIRTUAL JOYSTICK //////////////////////////////////////////////////
bool MyMfdHttpServer::registerVirtualJoystick(uint id, QString &errorMessage)
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






// PROCESS FILE ///////////////////////////////////////////////////////////////
HttpPromise MyMfdHttpServer::processFile(HttpDataPtr data)
{
	// regex = ^/file/(\\w|.|-)+/(\\w|.|-)+$
	auto match = data->state["match"].value<QRegularExpressionMatch>();
	QStringList urlSplit = match.captured(0).split('/',Qt::SkipEmptyParts);
	QString name = urlSplit[1];
	QString fileName = urlSplit[2];
	// qDebug() << "processFile(" << name << "," << fileName << ")";

	if (!m_resourcesDir.contains(name)) {
		data->response->setStatus(HttpStatus::BadRequest);
		return HttpPromise::resolve(data);
	}
	
	if (fileName == "gui.html")
	{
		QFile file{m_resourcesDir[name] + "/gui.html"};
		if (!file.open(QIODevice::ReadOnly)) {
			data->response->setStatus(HttpStatus::NotFound);
			return HttpPromise::resolve(data);
		}
		QString html = file.readAll();
		file.close();
		html.replace("@name@",name);
		data->response->setStatus(HttpStatus::Ok, html, "text/html");
		return HttpPromise::resolve(data);
	}
	else
	{
		QString filePath = m_resourcesDir[name] + "/" + fileName;
		bool ok = data->response->sendFile(filePath, {}, "utf-8");
		data->response->setStatus(ok ? HttpStatus::Ok : HttpStatus::NotFound);
		return HttpPromise::resolve(data);
	}
}

// PROCESS KEY ////////////////////////////////////////////////////////////////
HttpPromise MyMfdHttpServer::processKey(HttpDataPtr data)
{
	// regex = ^/key/\\w+(\\+\\w+)?/\\d$
	auto match = data->state["match"].value<QRegularExpressionMatch>();
	QStringList urlSplit = match.captured(0).split('/',Qt::SkipEmptyParts);
	QString keyStr = urlSplit[1];
	bool bDown = (urlSplit[2].toUInt() != 0);
	// qDebug() << "processKey(" << keyStr << "," << bDown << ")";

	if (!keyStrokeGenerator.generateKeyStroke(keyStr,bDown)) {
		data->response->setStatus(HttpStatus::PreconditionFailed, QString{"Incorrect key name"}, "text/plain");
		return HttpPromise::resolve(data);
	}

	data->response->setStatus(HttpStatus::NoContent);
	return HttpPromise::resolve(data);
}

// PROCESS BUTTON /////////////////////////////////////////////////////////////
HttpPromise MyMfdHttpServer::processButton(HttpDataPtr data)
{
	// regex = ^/button/\\d+/\\d+/\\d$
	auto match = data->state["match"].value<QRegularExpressionMatch>();
	QStringList urlSplit = match.captured(0).split('/',Qt::SkipEmptyParts);
	uint vji = urlSplit[1].toUInt();
	uint button = urlSplit[2].toUInt();
	bool bPressed = (urlSplit[3].toUInt() != 0);
	// qDebug() << "processButton(" << vji << "," << button << "," << bPressed << ")";

	auto f = [vji] (const std::unique_ptr<VirtualJoystick> &up) {return up->id() == vji;};
	auto result = std::find_if(m_virtualJoysticks.begin(),m_virtualJoysticks.end(),f);
	if (result == m_virtualJoysticks.end()) {
		data->response->setStatus(HttpStatus::PreconditionFailed, QString{"Virtual joystick not acquire"}, "text/plain");
		return HttpPromise::resolve(data);
	}

	VirtualJoystick *vj = result->get();
	if (!vj->setButton(button,bPressed)) {
		data->response->setStatus(HttpStatus::Forbidden, QString{"Button number is too big"}, "text/plain");
		return HttpPromise::resolve(data);
	}
	
	vj->flush();
	data->response->setStatus(HttpStatus::NoContent);
	return HttpPromise::resolve(data);
}

// PROCESS AXIS ///////////////////////////////////////////////////////////////
HttpPromise MyMfdHttpServer::processAxis(HttpDataPtr data)
{
	// regex = ^/axis/\\d+/\\d+/\\d(.\\d+)?$
	auto match = data->state["match"].value<QRegularExpressionMatch>();
	QStringList urlSplit = match.captured(0).split('/',Qt::SkipEmptyParts);
	uint vji = urlSplit[1].toUInt();
	uint axis = urlSplit[2].toUInt();
	float value = (urlSplit[3].toFloat());
	// qDebug() << "processAxis(" << vji << "," << axis << "," << value << ")";

	auto f = [vji] (const std::unique_ptr<VirtualJoystick> &up) {return up->id() == vji;};
	auto result = std::find_if(m_virtualJoysticks.begin(),m_virtualJoysticks.end(),f);
	if (result == m_virtualJoysticks.end()) {
		data->response->setStatus(HttpStatus::PreconditionFailed, QString{"Virtual joystick not acquired"}, "text/plain");
		return HttpPromise::resolve(data);
	}
	
	VirtualJoystick *vj = result->get();
	if (!vj->setAxis(axis,value)) {
		data->response->setStatus(HttpStatus::Forbidden, QString{"Axis number is too big"}, "text/plain");
		return HttpPromise::resolve(data);
	}
	
	vj->flush();
	data->response->setStatus(HttpStatus::NoContent);
	return HttpPromise::resolve(data);
}

// PROCESS POV ////////////////////////////////////////////////////////////////
HttpPromise MyMfdHttpServer::processPov(HttpDataPtr data)
{
	// regex = ^/pov/\\d+/\\d+/\\d(.\\d+)?$
	auto match = data->state["match"].value<QRegularExpressionMatch>();
	QStringList urlSplit = match.captured(0).split('/',Qt::SkipEmptyParts);
	uint vji = urlSplit[1].toUInt();
	uint pov = urlSplit[2].toUInt();
	float value = (urlSplit[3].toFloat());
	// qDebug() << "processPov(" << vji << "," << pov << "," << value << ")";

	auto f = [vji] (const std::unique_ptr<VirtualJoystick> &up) {return up->id() == vji;};
	auto result = std::find_if(m_virtualJoysticks.begin(),m_virtualJoysticks.end(),f);
	if (result == m_virtualJoysticks.end()) {
		data->response->setStatus(HttpStatus::PreconditionFailed, QString{"Virtual joystick not acquired"}, "text/plain");
		return HttpPromise::resolve(data);
	}
	
	VirtualJoystick *vj = result->get();
	if (!vj->setPov(pov,value)) {
		data->response->setStatus(HttpStatus::Forbidden, QString{"Pov number is too big"}, "text/plain");
		return HttpPromise::resolve(data);
	}
	
	vj->flush();
	data->response->setStatus(HttpStatus::NoContent);
	return HttpPromise::resolve(data);
}

