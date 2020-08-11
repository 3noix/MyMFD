#ifndef HTTP_SERVER
#define HTTP_SERVER


#include <QMap>
#include <QHttpServerResponse>
#include "KeyStrokeGenerator.h"
#include <vector>
#include <memory>
class QHttpServer;
class VirtualJoystick;


class HttpServer
{
	public:
		HttpServer();
		HttpServer(const HttpServer &other) = delete;
		HttpServer(HttpServer &&other) = delete;
		HttpServer& operator=(const HttpServer &other) = delete;
		HttpServer& operator=(HttpServer &&other) = delete;
		~HttpServer();
		
		bool listen();
		bool registerRemoteController(const QString &name, const QString &dirPath, QString &errorMessage);
		bool registerVirtualJoystick(uint id, QString &errorMessage);
		QString url(const QString &name) const;
		void closeServer();
		
		static QString ethernetLocalIpAddress(bool ipv6 = false);
		
		
	private:
		QHttpServerResponse processFile(const QString &name, const QString &fileName);
		QHttpServerResponse processKey(const QString &keyStr, bool bDown);
		QHttpServerResponse processButton(uint vji, uint button, bool bPressed);
		QHttpServerResponse processAxis(uint vji, uint axis, float value);
		QHttpServerResponse processPov(uint vji, uint pov, float value);
		
		QHttpServer *m_httpServer;
		QMap<QString,QString> m_resourcesDir;
		std::vector<std::unique_ptr<VirtualJoystick>> m_virtualJoysticks;
		KeyStrokeGenerator keyStrokeGenerator;
};


#endif

