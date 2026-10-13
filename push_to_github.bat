@echo off
echo ===================================================
echo     Pushing Vehicle Rental System to GitHub
echo ===================================================
set "PATH=C:\Users\mansi\.gemini\antigravity\scratch\mingit\cmd;%PATH%"

git remote -v
echo.
echo Pushing branch 'main' to origin...
git push -u origin main

if %ERRORLEVEL% EQU 0 (
    echo.
    echo [SUCCESS] Project successfully pushed to GitHub!
    echo Visit: https://github.com/mansisharma11240104-a11y/Vehicle-Rental-System
) else (
    echo.
    echo [NOTE] If prompted for credentials:
    echo   Username: your GitHub username (mansisharma11240104-a11y)
    echo   Password: your GitHub Personal Access Token (PAT)
)
pause
