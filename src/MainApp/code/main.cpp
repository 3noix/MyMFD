#include <QCoreApplication>
#include <iostream>


int main(int argc, char *argv[])
{
	QCoreApplication app{argc,argv};
	QStringList args = app.arguments();
	args.removeFirst();
	
	std::cout << "Appuyez sur une touche pour terminer" << std::endl;
	std::cin.get();
	return 0;
	
	//return app.exec();
}

