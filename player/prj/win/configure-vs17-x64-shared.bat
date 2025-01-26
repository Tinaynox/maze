@echo off
cd %~dp0
call var.bat


set CMAKELISTS_DIR=%~dp0..\..\

set PRJ_DIR="%PRJ_ROOT_DIR%\%PROJECT_NAME%\prj.vs17-x64-shared"
set BUILD_DIR="%PRJ_ROOT_DIR%\%PROJECT_NAME%\build.x64"


if not exist %PRJ_DIR% (
    mkdir %PRJ_DIR%
)
cd %PRJ_DIR%


if not exist %BUILD_DIR% (
    mkdir %BUILD_DIR%
)


cmake %CMAKELISTS_DIR% -G "Visual Studio 15 2017 Win64"                    ^
    -DBUILD_SHARED_LIBS=ON                                    


pause