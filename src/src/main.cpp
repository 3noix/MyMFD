#include <QCoreApplication>
#include <QFileInfo>
#include <iostream>
#include "MyMfdHttpServer.h"

struct RemoteRegisteringData
{
	QString name;
	QString dirPath;
};

unsigned short getServerPort(const QStringList &args);
bool getNoKeyStrokeGeneration(const QStringList &args);
std::vector<RemoteRegisteringData> getRemoteData(const QStringList &args);
std::vector<uint> getVjoyIndexes(const QStringList &args);


int main(int argc, char *argv[])
{
	QCoreApplication app{argc,argv};
	QStringList args = app.arguments();
	args.removeFirst();

	// parse arguments
	unsigned short port = getServerPort(args);
	bool noKeyStroke = getNoKeyStrokeGeneration(args);
	std::vector<RemoteRegisteringData> remoteDirs = getRemoteData(args);
	std::vector<uint> vjoyIndexes = getVjoyIndexes(args);

	// configure server
	HttpServerConfig config;
	config.host = QHostAddress::Any;
	config.port = port;
	config.verbosity = HttpServerConfig::Verbose::Critical;
	MyMfdHttpServer httpServer{config, noKeyStroke};
	QString errorMessage;
	
	// register resources dirs
	if (remoteDirs.size() == 0)
	{
		std::cout << "No remote dir specified" << std::endl;
		std::cout << "Press a key to quit" << std::endl;
		std::cin.get();
		return 1;
	}
	for (const RemoteRegisteringData &data : remoteDirs)
	{
		if (!httpServer.registerRemoteController(data.name,data.dirPath,errorMessage))
		{
			std::cout << "Failed to register remote" << qPrintable(data.name) << ": " << qPrintable(errorMessage) << std::endl;
			std::cout << "Press a key to quit" << std::endl;
			std::cin.get();
			return 2;
		}
		
		QString message = "Remote \"" + data.name + "\" accessible at \"" + httpServer.url(data.name) + "\"";
		std::cout << qPrintable(message) << std::endl;
	}
	
	// acquire virtual joysticks if needed
	for (int index : vjoyIndexes)
	{
		if (!httpServer.registerVirtualJoystick(index,errorMessage))
		{
			std::cout << "Failed to acquire virtual joystick #" << index << ": " << qPrintable(errorMessage) << std::endl;
			std::cout << "Press a key to quit" << std::endl;
			std::cin.get();
			return 3;
		}
		std::cout << "Virtual joystick #" << index << " acquired" << std::endl;
	}
	
	httpServer.listen();
	std::cout << "Press Ctrl+C to terminate" << std::endl;
	return app.exec();
}


// GET SERVER PORT ////////////////////////////////////////////////////////////
unsigned short getServerPort(const QStringList &args)
{
	QRegExp regex{"^-port=(\\d+)$"};
	for (const QString &arg : args)
	{
		if (regex.exactMatch(arg))
		{
			bool ok = false;
			unsigned short port = regex.cap(1).toUShort(&ok);
			return ok ? port : 8080;
		}
	}

	return 8080;
}

// GET NO KEY STROKE GENERATION ///////////////////////////////////////////////
bool getNoKeyStrokeGeneration(const QStringList &args)
{
	return args.contains("-nokeystroke");
}

// GET REMOTE DATA ////////////////////////////////////////////////////////////
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


// GET VJOY INDEXES ///////////////////////////////////////////////////////////
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

