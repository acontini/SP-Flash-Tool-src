@set path=%path%;C:\Program Files\Microsoft Visual Studio 9.0\VC\bin;C:\Program Files\Microsoft SDKs\Windows\v6.0A\bin;D:\env
@set WORKING_DIR=%cd%
@set TOOLS_DIR=%WORKING_DIR%\Build\Tools\Windows
@rem %1: build type(debug or release) %2: clean or not
@set BUILD_FLAG=%1
@if not defined BUILD_FLAG @set BUILD_FLAG=debug
make.exe HOST=windows BUILD_TYPE=%BUILD_FLAG% %2
@echo off
if %errorlevel%==0 (@echo off) else (exit %errorlevel%) 