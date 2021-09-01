echo off
echo Setting up environment for cleaning projects...
set PATH=%~dp0\utils\CleanQtProjects\release;%PATH%
echo.
echo.


echo cleaning in progress...
echo.
echo.
CleanQtProjects.exe %~dp0\3rdparty\HttpServer\src
CleanQtProjects.exe %~dp0\3rdparty\HttpServer\test
CleanQtProjects.exe %~dp0\3rdparty\HttpServer\mytest
CleanQtProjects.exe %~dp0\src
echo.
echo.


echo removing CleanQtProjects manually
del %~dp0\utils\CleanQtProjects\.qmake.stash
del %~dp0\utils\CleanQtProjects\Makefile
del %~dp0\utils\CleanQtProjects\Makefile.Debug
del %~dp0\utils\CleanQtProjects\Makefile.Release
rmdir /S /Q %~dp0\utils\CleanQtProjects\debug
rmdir /S /Q %~dp0\utils\CleanQtProjects\release
echo.
echo.


echo cleaning up bin directory
cd bin
rmdir /S /Q bearer
rmdir /S /Q iconengines
rmdir /S /Q imageformats
rmdir /S /Q platforms
rmdir /S /Q styles
rmdir /S /Q translations
rmdir /S /Q ControllersPlugins
del *.exe
del *.dll
cd ..
echo.
echo.


pause

