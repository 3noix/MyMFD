#ifndef MY_MFD_HTTP_SERVER
#define MY_MFD_HTTP_SERVER


#include "HttpServer.h"
#include "VirtualJoystick.h"
#include "KeyStrokeGenerator.h"
#include <QMap>
#include <vector>
#include <memory>


class MyMfdHttpServer : public HttpServer
{
	public:
		MyMfdHttpServer(const HttpServerConfig &config);
		MyMfdHttpServer(const MyMfdHttpServer &other) = delete;
		MyMfdHttpServer(MyMfdHttpServer &&other) = delete;
		MyMfdHttpServer& operator=(const MyMfdHttpServer &other) = delete;
		MyMfdHttpServer& operator=(MyMfdHttpServer &&other) = delete;
		~MyMfdHttpServer() = default;
		
		bool registerRemoteController(const QString &name, const QString &dirPath, QString &errorMessage);
		bool registerVirtualJoystick(uint id, QString &errorMessage);
		QString url(const QString &name) const;
		
		static QString ethernetLocalIpAddress(bool ipv6 = false);
		
		
	private:
		HttpPromise processFile(HttpDataPtr data);
		HttpPromise processKey(HttpDataPtr data);
		HttpPromise processButton(HttpDataPtr data);
		HttpPromise processAxis(HttpDataPtr data);
		HttpPromise processPov(HttpDataPtr data);
		HttpPromise processLog(HttpDataPtr data);
		
		QMap<QString,QString> m_resourcesDir;
		std::vector<std::unique_ptr<VirtualJoystick>> m_virtualJoysticks;
		KeyStrokeGenerator keyStrokeGenerator;
};


#endif

