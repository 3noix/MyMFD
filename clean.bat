echo off
echo Setting up environment for cleaning projects...
set PATH=%~dp0\src\CleanQtProjects\release;%PATH%
echo.
echo.


echo cleaning in progress...
echo.
echo.
CleanQtProjects.exe %~dp0\src\MainApp
echo.
echo.


echo removing CleanQtProjects manually
del %~dp0\src\CleanQtProjects\.qmake.stash
del %~dp0\src\CleanQtProjects\Makefile
del %~dp0\src\CleanQtProjects\Makefile.Debug
del %~dp0\src\CleanQtProjects\Makefile.Release
rmdir /S /Q %~dp0\src\CleanQtProjects\debug
rmdir /S /Q %~dp0\src\CleanQtProjects\release
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

