@echo off

set PRJ_DIR="%PRJ_ROOT_DIR%\%PROJECT_NAME%\prj.emscripten-static"
set BUILD_DIR="%PRJ_ROOT_DIR%\%PROJECT_NAME%\build.emscripten"

if not exist %PRJ_DIR% (
    mkdir %PRJ_DIR%
)
cd %PRJ_DIR%


if not exist %BUILD_DIR% (
    mkdir %BUILD_DIR%
)


call emsdk_env.bat

if "%EMSDK%"=="" (
    echo ERROR: EMSDK not set! Run this script from the directory containing emsdk_env.bat or fix your setup.
    pause
    exit /b 1
)

:: Force EMSCRIPTEN variable (modern emsdk doesn't always set it)
set EMSCRIPTEN=%EMSDK%\upstream\emscripten
echo Using EMSCRIPTEN = %EMSCRIPTEN%

:: Verify
if not exist "%EMSCRIPTEN%\emcc.exe" (
    echo ERROR: emcc.bat not found!
    echo Expected: %EMSCRIPTEN%\emcc.exe
    pause
    exit /b 1
)

call %~dp0\assembly-resources-x1.0-nc.bat

cmake %CMAKELISTS_DIR% -G "MinGW Makefiles"                                    ^
    -DCMAKE_TOOLCHAIN_FILE=%MAZE_ENGINE_DIR%\cmake\toolchains\Emscripten.cmake                 ^
    -DBUILD_SHARED_LIBS=OFF                                            ^
    -DCMAKE_BUILD_TYPE=Release ^
	-DCMAKE_POLICY_VERSION_MINIMUM=3.5