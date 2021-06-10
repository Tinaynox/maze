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
set EMSCRIPTEN=%EMSDK%/upstream/emscripten/

rem call emsdk activate latest

call %~dp0\assembly-resources-x1.0-nc.bat

cmake %CMAKELISTS_DIR% -G "MinGW Makefiles"                                    ^
    -DCMAKE_TOOLCHAIN_FILE=%MAZE_ENGINE_DIR%\cmake\toolchains\Emscripten.cmake                 ^
    -DBUILD_SHARED_LIBS=OFF                                            ^
    -DCMAKE_BUILD_TYPE=Release