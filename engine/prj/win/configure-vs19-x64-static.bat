@echo off
cd %~dp0
call var.bat

set PRJ_DIR="%PRJ_ROOT_DIR%\%PROJECT_NAME%\prj.vs19-x64-static"

if not exist %PRJ_DIR% (
    mkdir %PRJ_DIR%
)
cd %PRJ_DIR%


set CMAKELISTS_DIR=%~dp0..\..\
cmake %CMAKELISTS_DIR% -G "Visual Studio 16 2019" -A x64 ^
    -DBUILD_SHARED_LIBS=OFF                                    

pause
