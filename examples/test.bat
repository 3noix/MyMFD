echo off

REM Computes the path to the MyMFD executable
REM "%~dp0" designates the directory of this bat file
set EXECUTABLE=%~dp0\..\bin\MyMFD.exe

REM Computes the path to the dir containing the files (html, css, js, ...)
set REMOTEDIR1=%~dp0\test

REM If you want to be able to sollicitate vJoy devices, they must be acquired by the app.
REM To do so, you have to provide them as arguments: for vjoy devices #1, 3 and 6, type:
REM -vjoy=1+3+6

REM To specify the directory to your web files, you have to use the "-remote=(xx,yy)" arg.
REM You can specify several ones this way: "-remote=(aa,bb) -remote=(cc,dd)
%EXECUTABLE% -vjoy=1 -remote=(name,%REMOTEDIR1%)

echo.
pause

