@echo off

set "REPO_DIR=%~dp0"
set "BUILD_DIR=%REPO_DIR%build"
set "SOURCES_DIR=%REPO_DIR%Sources"

if exist "%BUILD_DIR%" (
	echo "Remove %BUILD_DIR%"
	del /S /Q /F "%BUILD_DIR%"
	for /f %%f in ('dir /ad /b "%BUILD_DIR%"') do rd /s /q "%BUILD_DIR%"\%%f
)

cd "%REPO_DIR%\build"
cmake "%REPO_DIR%\Sources"
rem cmake "%REPO_DIR%\Sources" -DHUNTER_STATUS_DEBUG=ON
cd "%REPO_DIR%"