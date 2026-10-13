@echo off
if not exist vehicle_rental.exe (
    echo vehicle_rental.exe not found. Building first...
    call build.bat
)
vehicle_rental.exe
pause
