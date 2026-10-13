@echo off
echo ===================================================
echo   Compiling Wheels & Ways Vehicle Rental System
echo ===================================================

echo [1/2] Compiling Terminal CLI (vehicle_rental.exe)...
g++ -std=c++14 -Wall -Iinclude src/Vehicle.cpp src/TwoWheeler.cpp src/FourWheeler.cpp src/RentalSystem.cpp src/main.cpp -o vehicle_rental.exe

echo [2/2] Compiling Web Server (web_server.exe)...
g++ -std=c++14 -Wall -Iinclude src/Vehicle.cpp src/TwoWheeler.cpp src/FourWheeler.cpp src/web_server.cpp -lws2_32 -o web_server.exe

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ===================================================
    echo  [SUCCESS] All components built successfully!
    echo   - Terminal CLI : run 'run.bat' or .\vehicle_rental.exe
    echo   - Web Server   : run 'start_web.bat' or visit http://localhost:8080/
    echo ===================================================
) else (
    echo [ERROR] Build failed. Check errors above.
)
pause
