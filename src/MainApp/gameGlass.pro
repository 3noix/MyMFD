CONFIG(debug, debug|release) {
	MODE = debug
}
CONFIG(release, debug|release) {
	MODE = release
}


TEMPLATE = app
TARGET = MyMFD
CONFIG += c++11
DESTDIR = ../../bin
OBJECTS_DIR = $$MODE/objects
MOC_DIR = $$MODE/moc
QT += core xmlpatterns network httpserver


win32-g++:LIBS += ../../3rdparty/vJoy218SDK-291116/SDK/lib/vJoyInterface.lib
win32-msvc*:LIBS += ../../3rdparty/vJoy218SDK-291116/SDK/lib/amd64/vJoyInterface.lib


HEADERS +=  CODE/VirtualJoystick.h


SOURCES +=  CODE/main.cpp \
			CODE/VirtualJoystick.cpp \
			CODE/RemoteController.cpp \
			CODE/UjpsHttpServer.cpp \
			CODE/VirtualEventsQueue.cpp


RC_FILE += icone.rc

