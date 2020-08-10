#include <QCoreApplication>
#include <QFileInfo>
#include <iostream>
#include "HttpServer.h"

struct RemoteRegisteringData
{
	QString name;
	QString dirPath;
};

std::vector<RemoteRegisteringData> getRemoteData(const QStringList &args);
std::vector<uint> getVjoyIndexes(const QStringList &args);


int main(int argc, char *argv[])
{
	QCoreApplication app{argc,argv};
	QStringList args = app.arguments();
	args.removeFirst();
	
	HttpServer httpServer;
	QString errorMessage;
	
	// register resources dirs
	for (const RemoteRegisteringData &data : getRemoteData(args))
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
	for (int index : getVjoyIndexes(args))
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


std::vector<RemoteRegisteringData> getRemoteData(const QStringList &args)
{
	std::vector<RemoteRegisteringData> data;
	
	for (const QString &arg : args)
	{
		if (arg.startsWith("-remote=(") && arg.endsWith(")") && arg.count('(') == 1 && arg.count(',') == 1 && arg.count(')') == 1)
		{
			QStringList temp = arg.mid(9,arg.size()-10).split(',',Qt::KeepEmptyParts);
			if (temp.size() != 2) {continue;}
			RemoteRegisteringData rrd{temp[0],QFileInfo{temp[1]}.absoluteFilePath()};
			data.push_back(rrd);
		}
	}
	
	return data;
}


std::vector<uint> getVjoyIndexes(const QStringList &args)
{
	std::vector<uint> indexes;
	
	for (const QString &arg : args)
	{
		if (arg.startsWith("-vjoy=") && arg.size() > 6)
		{
			for (const QString &temp : arg.mid(6).split('+',Qt::SkipEmptyParts))
			{
				bool bok = true;
				uint index = temp.toUInt(&bok);
				if (bok) {indexes.push_back(index);}
			}
		}
	}
	
	std::sort(indexes.begin(),indexes.end());
	auto newEnd = std::unique(indexes.begin(),indexes.end());
	indexes.erase(newEnd,indexes.end());
	return indexes;
}

