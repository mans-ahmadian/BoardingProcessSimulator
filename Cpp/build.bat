@echo off
REM Create the build directory if it doesn't exist
if not exist build (
mkdir build
)

REM Navigate to the build directory
cd build

REM Run CMake to configure the project
cmake ..

REM Build the project
cmake --build .
