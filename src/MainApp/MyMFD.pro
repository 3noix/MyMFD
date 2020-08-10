CONFIG(debug, debug|release) {
	MODE = debug
}
CONFIG(release, debug|release) {
	MODE = release
}


TEMPLATE = app
TARGET = MyMFD
CONFIG += c++11 console
DESTDIR = ../../bin
OBJECTS_DIR = $$MODE/objects
MOC_DIR = $$MODE/moc
QT += core xmlpatterns network httpserver


win32-g++:LIBS += ../../3rdparty/vJoy218SDK-291116/SDK/lib/vJoyInterface.lib
win32-msvc*:LIBS += ../../3rdparty/vJoy218SDK-291116/SDK/lib/amd64/vJoyInterface.lib


SOURCES +=  CODE/main.cpp \
			CODE/VirtualJoystick.cpp \
			CODE/VirtualJoystickPrivate.cpp \
			CODE/HttpServer.cpp \
			CODE/KeyStrokeGenerator.cpp


RC_FILE += icone.rc

