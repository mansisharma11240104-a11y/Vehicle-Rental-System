@echo off
echo ===================================================
echo   Starting Vehicle Rental Web Server on Localhost
echo ===================================================

if not exist web_server.exe (
    echo Compiling web_server.exe...
    g++ -std=c++14 -Wall -Iinclude src/Vehicle.cpp src/TwoWheeler.cpp src/FourWheeler.cpp src/web_server.cpp -lws2_32 -o web_server.exe
)

echo.
echo Server running at http://localhost:8080/
echo Opening default web browser...
start http://localhost:8080/
echo.
echo Press Ctrl+C in this window to stop the server.
web_server.exe
pause
