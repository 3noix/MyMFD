echo off
echo.
cd %~dp0


If not exist bin mkdir bin


REM compilation and deployment of CleanQtProjects
echo compiling CleanQtProjects
cd utils/CleanQtProjects
qmake
mingw32-make release
cd release
windeployqt CleanQtProjects.exe
cd ../../..
echo.
echo.


REM compilation of static lib HttpServer
echo compiling HttpServer
cd 3rdparty/HttpServer/src
qmake
mingw32-make release
cd ../../..
echo.
echo.


REM compilation and deployment of main app
echo compiling MainApp
cd src
qmake
mingw32-make release
cd ..
echo.
echo.


REM deployment
cd bin
windeployqt MyMFD.exe
cd ..
copy /Y 3rdparty\vJoy218SDK-291116\SDK\lib\vJoyInterface.dll bin
copy /Y 3rdparty\HttpServer\src\release\httpServer.dll bin


echo finished
pause

