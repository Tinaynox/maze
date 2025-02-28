@echo off
cd %~dp0

set CMAKELISTS_DIR=%~dp0
set PRJ_DIR="prj"

if not exist %PRJ_DIR% (
    mkdir %PRJ_DIR%
)
cd %PRJ_DIR%

cmake %CMAKELISTS_DIR% -G "Visual Studio 16 2019" -A x64