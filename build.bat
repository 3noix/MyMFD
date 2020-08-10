echo off
echo.


If not exist bin mkdir bin


REM compilation and deployment of CleanQtProjects
cd %~dp0\src\CleanQtProjects
call build_CleanQtProjects.bat
cd ../..


REM compilation and deployment of UJPS main app
cd src/MainApp
call build_MainApp.bat
cd ../..


cd bin
windeployqt MyMFD.exe
cd ..
copy /Y 3rdparty\vJoy218SDK-291116\SDK\lib\vJoyInterface.dll bin


echo finished
pause

