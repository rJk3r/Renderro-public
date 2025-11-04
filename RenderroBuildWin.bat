@echo off
REM Renderro - Windows Build Script
REM Builds the project using CMake without generating Visual Studio solution files.
REM Usage: RenderroBuildWin.bat [Debug|Release] [clean] [run]

setlocal enabledelayedexpansion

echo ========================================
echo    Renderro - Windows Build Script
echo ========================================

REM Detect build type (Debug/Release)
if "%1"=="" (
    set BUILD_TYPE=Debug
) else (
    set BUILD_TYPE=%1
)

REM Check if clean is requested
set CLEAN_BUILD=false
if "%2"=="clean" set CLEAN_BUILD=true
if "%3"=="clean" set CLEAN_BUILD=true

REM Check if run is requested
set RUN_AFTER_BUILD=false
if "%2"=="run" set RUN_AFTER_BUILD=true
if "%3"=="run" set RUN_AFTER_BUILD=true

echo Build type: %BUILD_TYPE%
echo Clean build: %CLEAN_BUILD%
echo Run after build: %RUN_AFTER_BUILD%
echo.

REM Check if CMake is installed
where cmake >nul 2>nul
if errorlevel 1 (
    echo ERROR: CMake is not installed or not in PATH
    echo Please install CMake: https://cmake.org/download/
    pause
    exit /b 1
)

REM Check if Visual Studio compiler is available, if not try to initialize VS environment
set VS_INITIALIZED=false
where cl >nul 2>nul
if errorlevel 1 (
    echo Visual Studio compiler not found in PATH. Attempting to locate and initialize Visual Studio...
    set VSWHERE_PATH=
    where vswhere.exe >nul 2>nul
    if errorlevel 1 goto :FindVSWhere
    for /f "usebackq delims=" %%I in (`where vswhere.exe`) do set "VSWHERE_PATH=%%I"
    :FindVSWhere
    REM Use delayed expansion for ProgramFiles(x86) to avoid parsing parentheses inside this block
    if "!VSWHERE_PATH!"=="" (
        if exist "!ProgramFiles(x86)!\Microsoft Visual Studio\Installer\vswhere.exe" (
            set "VSWHERE_PATH=!ProgramFiles(x86)!\Microsoft Visual Studio\Installer\vswhere.exe"
        )
    )

    if not "!VSWHERE_PATH!"=="" (
        for /f "usebackq delims=" %%I in (`"!VSWHERE_PATH!" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do set "VSINSTALL=%%I"
    )

    if not "!VSINSTALL!"=="" (
        set "VSDEVCMD=!VSINSTALL!\Common7\Tools\VsDevCmd.bat"
        set "VCVARS=!VSINSTALL!\VC\Auxiliary\Build\vcvarsall.bat"
        if exist "!VSDEVCMD!" (
            echo Calling VsDevCmd.bat to initialize developer environment...
            call "!VSDEVCMD!" -arch=amd64
            where cl >nul 2>nul
            set VS_CHECK_ERR=!ERRORLEVEL!
            if !VS_CHECK_ERR! equ 0 (
                set VS_INITIALIZED=true
            )
        ) else if exist "!VCVARS!" (
            echo Calling vcvarsall.bat to initialize developer environment...
            call "!VCVARS!" amd64
            where cl >nul 2>nul
            set VS_CHECK_ERR=!ERRORLEVEL!
            if !VS_CHECK_ERR! equ 0 (
                set VS_INITIALIZED=true
            )
        )
    )
    
    if "!VS_INITIALIZED!"=="false" (
        echo WARNING: Could not initialize Visual Studio environment automatically.
        echo Make sure you have Visual Studio or Build Tools installed.
        echo You may need to run this script from a Developer Command Prompt.
        echo.
    )
)

REM Prefer Ninja generator to avoid Visual Studio solution creation
set GENERATOR=
where ninja >nul 2>nul
if errorlevel 1 goto :CheckNMake
set GENERATOR=Ninja
goto :GeneratorFound

:CheckNMake
where nmake >nul 2>nul
if errorlevel 1 goto :NoGenerator
set GENERATOR=NMake Makefiles
goto :GeneratorFound

:NoGenerator
echo ERROR: Could not find Ninja or nmake in PATH.
echo Install Ninja (https://ninja-build.org/) or run this script from a Developer Command Prompt.
pause
exit /b 1

:GeneratorFound

echo Using CMake generator: %GENERATOR%
echo.

REM Check for GLM headers inside libs/glm (project expects headers at libs/glm/glm/...)
if not exist "libs\glm\glm\glm.hpp" (
    if not exist "libs\glm\glm.hpp" (
        echo ERROR: GLM headers not found under libs\glm.
        echo The project requires the GLM header-only library (https://github.com/g-truc/glm).
        echo Options to fix:
        echo  1) Install via vcpkg and integrate: (example)
        echo     > git clone https://github.com/microsoft/vcpkg.git
        echo     > .\vcpkg\bootstrap-vcpkg.bat
        echo     > .\vcpkg\vcpkg install glm
        echo     Then follow vcpkg integration instructions so CMake can find glm.
        echo  2) Or download GLM and place the headers at libs\glm\glm\*.hpp (i.e. libs\glm\glm\glm.hpp should exist).
        echo After installing, re-run this script.
        pause
        exit /b 1
    )
)

REM Create build directory
set BUILD_DIR=build\%BUILD_TYPE%
if not exist "%BUILD_DIR%" (
    echo Creating build directory: %BUILD_DIR%
    mkdir "%BUILD_DIR%"
)

REM Clean build if requested
if "%CLEAN_BUILD%"=="true" (
    echo Cleaning build directory...
    if exist "%BUILD_DIR%" (
        rmdir /s /q "%BUILD_DIR%"
        mkdir "%BUILD_DIR%"
    )
)

echo.
echo ========================================
echo Configuring with CMake...
echo ========================================
cmake -S . -B "%BUILD_DIR%" -G "%GENERATOR%" -DCMAKE_BUILD_TYPE=%BUILD_TYPE%
if errorlevel 1 (
    echo ERROR: Failed to configure with CMake
    pause
    exit /b 1
)

echo.
echo ========================================
echo Building Renderro...
echo ========================================
cmake --build "%BUILD_DIR%" --config %BUILD_TYPE% --parallel
if errorlevel 1 (
    echo ERROR: Build failed
    pause
    exit /b 1
)

echo.
echo ========================================
echo Build completed successfully!
echo ========================================
echo.
echo Build directory: %BUILD_DIR%

REM Known executable name from CMake: SimpleRenderer
set EXE_NAME=SimpleRenderer.exe

REM Try common locations for the built executable
if exist "%BUILD_DIR%\%EXE_NAME%" (
    set EXE_PATH=%BUILD_DIR%\%EXE_NAME%
) else if exist "%BUILD_DIR%\%BUILD_TYPE%\%EXE_NAME%" (
    set EXE_PATH=%BUILD_DIR%\%BUILD_TYPE%\%EXE_NAME%
) else if exist "%BUILD_DIR%\bin\%EXE_NAME%" (
    set EXE_PATH=%BUILD_DIR%\bin\%EXE_NAME%
) else (
    set EXE_PATH=
)

if "%EXE_PATH%"=="" (
    echo WARNING: Could not find %EXE_NAME% in expected output locations.
    echo You can run the executable manually from the build directory.
) else (
    echo Found executable: %EXE_PATH%
    if "%RUN_AFTER_BUILD%"=="true" (
        echo.
        echo Running %EXE_NAME%...
        start "" "%EXE_PATH%"
    )
)

echo.
echo Build script completed!
pause
exit /b 0