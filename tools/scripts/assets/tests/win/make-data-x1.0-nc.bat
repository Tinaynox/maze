@echo off
cd %~dp0
call var.bat


set ASSETS_DIR=%~dp0..\assets
set BUILD_DIR=%PRJ_ROOT_DIR%\%PROJECT_NAME%\build-x1.0-nc

if not exist %BUILD_DIR% (
    mkdir %BUILD_DIR%
)
cd %BUILD_DIR%

call %~dp0\assembly-assets-x1.0-nc.bat
call python %MAZE_ENGINE_DIR%\..\tools\scripts\assets\copy_resources.py --src %ASSETS_X1_NC_DIR% --dst %BUILD_DIR%\data
pause