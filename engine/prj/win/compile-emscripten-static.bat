@echo off
cd %~dp0
call var.bat

set PRJ_DIR="%PRJ_ROOT_DIR%\%PROJECT_NAME%\prj.emscripten-static"

if not exist %PRJ_DIR% (
    mkdir %PRJ_DIR%
)
cd %PRJ_DIR%

call mingw32-make

pause
