@echo off
set myconfig=Release

REM Create build directory if it doesn't exist
if not exist ".\kOS\build" (
    mkdir ".\kOS\build"
)

REM Change to the build directory
cd ".\kOS\build"

REM Configure the project
cmake -S ..\ -B . -DHOT_RELOAD_BUILD=off

REM Build the project in Release configuration
cmake --build . --config %myconfig% 

REM Open the application
cd ..\bin\%myconfig%

.\Kos_Editor.exe