#include <QCoreApplication>
#include <iostream>
#include "HttpServer.h"

struct RemoteRegisteringData
{
	QString name;
	QString dirPath;
};


int main(int argc, char *argv[])
{
	QCoreApplication app{argc,argv};
	QString appDir = QCoreApplication::applicationDirPath();
	QStringList args = app.arguments();
	args.removeFirst();
	
	HttpServer httpServer;
	QString errorMessage;
	
	// register resources dirs
	std::vector<RemoteRegisteringData> resourcesData{{"test1",appDir+"/../examples/test"}}; // TO BE COMPUTED FROM ARGUMENTS
	for (const RemoteRegisteringData &data : resourcesData)
	{
		if (!httpServer.registerRemoteController(data.name,data.dirPath,errorMessage))
		{
			std::cout << "Failed to register remote" << qPrintable(data.name) << ": " << qPrintable(errorMessage) << std::endl;
			std::cout << "Press a key to quit" << std::endl;
			std::cin.get();
			return 1;
		}
		
		QString message = "Remote \"" + data.name + "\" accessible at \"" + httpServer.url(data.name) + "\"";
		std::cout << qPrintable(message) << std::endl;
	}
	
	// acquire virtual joysticks if needed
	std::vector<uint> vjIndexes{1,2,3}; // TO BE COMPUTED FROM ARGUMENTS
	for (int index : vjIndexes)
	{
		if (!httpServer.registerVirtualJoystick(index,errorMessage))
		{
			std::cout << "Failed to acquire virtual joystick #" << index << ": " << qPrintable(errorMessage) << std::endl;
			std::cout << "Press a key to quit" << std::endl;
			std::cin.get();
			return 2;
		}
		std::cout << "Virtual joystick #" << index << " acquired" << std::endl;
	}
	
	httpServer.listen();
	std::cout << "Press Ctrl+C to terminate" << std::endl;
	return app.exec();
}

