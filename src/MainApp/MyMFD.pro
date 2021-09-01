CONFIG(debug, debug|release) {
	MODE = debug
}
CONFIG(release, debug|release) {
	MODE = release
}


TEMPLATE = app
TARGET = MyMFD
CONFIG += c++14 console
DESTDIR = ../../bin
OBJECTS_DIR = $$MODE/objects
MOC_DIR = $$MODE/moc
QT += core xmlpatterns network


win32-g++:LIBS += ../../3rdparty/vJoy218SDK-291116/SDK/lib/vJoyInterface.lib
win32-msvc*:LIBS += ../../3rdparty/vJoy218SDK-291116/SDK/lib/amd64/vJoyInterface.lib

INCLUDEPATH += ../../3rdparty/HttpServer/3rdparty/QtPromise/include
INCLUDEPATH += ../../3rdparty/HttpServer/src
win32-g++:LIBS += ../../3rdparty/HttpServer/src/$$MODE/libhttpServer.a
win32-msvc*:LIBS += ../../3rdparty/HttpServer/src/$$MODE/libhttpServer.lib


HEADERS +=  CODE/MyMfdHttpServer.h


SOURCES +=  CODE/main.cpp \
			CODE/VirtualJoystick.cpp \
			CODE/VirtualJoystickPrivate.cpp \
			CODE/MyMfdHttpServer.cpp \
			CODE/KeyStrokeGenerator.cpp


RC_FILE += icone.rc

