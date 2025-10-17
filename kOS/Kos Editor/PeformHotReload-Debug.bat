@echo off

set myconfig=Debug
REM Create build directory if it doesn't exist
if not exist "..\build" (
    mkdir "..\build"
)

REM Go to the DLL output folder
cd "..\bin\%myconfig%\SCRIPTS"

REM Delete all old PDB files
del /Q *.pdb

REM Change to the build directory
cd "..\..\..\build"

REM Configure the project
cmake -S ..\ -B . -DHOT_RELOAD_BUILD=ON

REM Build the project in Debug configuration
cmake --build . --config %myconfig% --target SCRIPTS
