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

del /f %PRJ_DIR%\bin\%PROJECT_NAME%.wasm
del /f %PRJ_DIR%\bin\%PROJECT_NAME%.js
del /f %PRJ_DIR%\bin\%PROJECT_NAME%.data

call mingw32-make

xcopy /y/q %PRJ_DIR%\bin\%PROJECT_NAME%.wasm %BUILD_DIR%
xcopy /y/q %PRJ_DIR%\bin\%PROJECT_NAME%.js %BUILD_DIR%
xcopy /y/q %PRJ_DIR%\bin\%PROJECT_NAME%.data %BUILD_DIR%
