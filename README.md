# MyMFD
## Overview

The goal of this project is to provide you a way to use your own GUI on your mobile phone or tablet as an input device for your PC games. Its main purpose is the same as Roccat Power Grid, LEA Extended Input and GameGlass.

The easier way to define your own GUI is to do it using HTML, CSS and Javascript (using the provided test example as a starting point). But you can also create it with the tech you want and just send HTTP requests to the PC application. The different ways to use it will be discussed in the paragraphs below.

The PC application can generate keyboard events and vJoy events (if vJoy is installed). vJoy is not mandatory, but if it is not installed, you will be limited to keyboard events. This version works only for Windows. To start, download the latest release.

You can download vJoy [here](http://vjoystick.sourceforge.net/site/index.php/download-a-install/download).


## Remark

This project was very quickly done by extracting a small part of the bigger [UJPS](https://github.com/3noix/UJPS) project. If you want to use your custom web GUI in conjunction with your joysticks, I suggest you to take a look at UJPS.


## Build the project

<b>Building the project is not needed to use it: the simpler way is to use the last provided binary release.</b> If you really want to build it, you will need to:
- Install [Qt](https://www.qt.io/download-qt-installer/). To avoid any Qt version and compilers issues, I recommend Qt 5.15.1 or 5.15.2 with MinGW 8.1.0 32-bit. In the installation / maintenance tool, you should at least check the following items:
```bash
+-- Qt
  +-- Qt 5.15.1
    +-- MinwGW 8.1.0 32-bit
+-- Developer and Designer Tools
  +-- MinwGW 8.1.0 32-bit
```
- run the file build.bat (adjustments will be needed if you don't have MinGW or didn't install it along Qt at the previous step)


## Use the PC app

The PC app is very basic, it is just a console app. To provide to the app the path to your GUI files (and the indexes of the vJoy devices to use), you have to use the arguments of the app. The easier way to do is to use a bat file. Take a look at the verbosely commented example bat file below (the lines starting with "REM" are comments):


```Bat
echo off

REM Computes the path to the MyMFD executable
REM "%~dp0" designates the directory of this bat file
set EXECUTABLE=%~dp0\..\bin\MyMFD.exe

REM Computes the path to the dir containing the files of the GUI (html, css, js, ...)
set REMOTEDIR1=%~dp0\test

REM If you want to be able to sollicitate vJoy devices, they must be acquired by the app.
REM To do so, you have to provide them as arguments: for vjoy devices #1, 3 and 6, type:
REM -vjoy=1+3+6

REM To specify the directory to your web files, you have to use the "-remote=(xx,yy)" arg.
REM You can specify several ones this way: "-remote=(aa,bb) -remote=(cc,dd)

REM Run the app with the arguments
%EXECUTABLE% -vjoy=1 -remote=(name,%REMOTEDIR1%)

echo.
pause
```

The PC app will display you a link in the console that you can use to access to the GUI using the web browser of your mobile phone or tablet (if connected to your local Wifi network).


## Create your own GUI

### First way: HTML, CSS with no Javascript

With this technique you will not need to write Javascript, you will just need to copy the Javascript file of the "template" example. This way is the simpler one, but it has some limitations:
- you can generate keystrokes only with html buttons
- you can generate vJoy buttons events only with html buttons
- you can generate vJoy axes events only with html ranges


For your html file, use the "gui.html" file of the "template" example as a starting point. As the result will not be graphically satisfactory, editing the css file will be a very good idea.


To generate a keystroke from a html button using **data-type="key"** and **data-key="theKeyYouWant"**:
```Html
<button type="button" data-type="key" data-key="A">a</button>
<button type="button" data-type="key" data-key="LShift+B">B</button>
```

To generate a vJoy button event from a html button, use **data-type="button"**, specify the index of the vJoy device in **data-vjoy** attribute and the number of the button in **data-number** attribute:
```Html
<button type="button" data-type="button" data-vjoy="1" data-number="1">1</button>
```

To generate a vJoy axis event from a html range, use **data-type="axis"**, specify the index of the vJoy device in **data-vjoy** attribute and the number of the button in **data-number** attribute:
```Html
<input type="range" data-type="axis" data-vjoy="1" data-number="1"/>
```



### Second way: HTML, CSS and more Javascript

If you feel too limited with the first way, you can drop the use of the "data-" attributes and use directly the Javascript functions "sendKey", "sendButton", "sendAxis" and "sendPov" (or their touch/mouse version) with the events of any html widget you want.


### Third way: Using HTTP requests

If you don't even want to use web files, you can use any tech you want and just send http requests:
```Html
<!-- xxx.xxx.xxx.xxx is the IP address of the PC -->
<!-- requests relative to keys -->
http://xxx.xxx.xxx.xxx:8080/key/{keyStr}/{down}
http://xxx.xxx.xxx.xxx:8080/key/LCtrl+B/1 (to press Left Ctrl and B keys)
http://xxx.xxx.xxx.xxx:8080/key/C/0 (to release C key)

<!-- requests relative to vJoy buttons -->
http://xxx.xxx.xxx.xxx:8080/button/{vJoyIndex}/{buttonNumber}/{pressed}
http://xxx.xxx.xxx.xxx:8080/button/1/2/1 (to press the second button of vJoy device #1)

<!-- requests relative to vJoy axes -->
http://xxx.xxx.xxx.xxx:8080/axis/{vJoyIndex}/{axisNumber}/{value}
http://xxx.xxx.xxx.xxx:8080/axis/3/1/0.35 (to set at 0.35 the value of the first axis of vJoy device #3)
// An axis value must be between -1 and +1.
```



## Other

You can consult the list of the currently supported keys in this file: *src/MainApp/code/KeyStrokeGenerator.cpp*

As this program can generate keystrokes from received http requests, and as the app currently listen to any input connection, it may be a **security issue**: if someone is able to establish a connection to your computer and does the right http requests, he/she can generate the keystrokes he/she wants. So close the PC app when you don't use it. I am also thinking about disabling the keystrokes on demand (using another argument) for people that would only want to use vJoy devices.