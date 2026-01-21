@echo off
REM EC Language Windows Build Script

echo ===================================
echo   EC Language Build Script
echo   Windows (MinGW)
echo ===================================
echo.

REM Check for GCC
where gcc >nul 2>nul
if %ERRORLEVEL% neq 0 (
    echo Error: GCC not found!
    echo Please install MinGW or add GCC to PATH.
    echo.
    echo Download MinGW from: https://winlibs.com/
    exit /b 1
)

echo Building EC interpreter...
gcc -Wall -O2 -o EC.exe src/ec.c -lm

if %ERRORLEVEL% equ 0 (
    echo.
    echo Build successful!
    echo.
    echo Executable: EC.exe
    echo.
    echo Testing...
    EC.exe --version
    echo.
    echo Run examples:
    echo   EC.exe examples\01_hello.ec
    echo   EC.exe examples\09_multiplication.ec
) else (
    echo.
    echo Build failed!
    exit /b 1
)
