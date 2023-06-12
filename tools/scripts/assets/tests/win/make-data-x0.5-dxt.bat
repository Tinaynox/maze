@echo off
cd %~dp0
call var.bat


set ASSETS_DIR=%~dp0..\assets
set BUILD_DIR=%PRJ_ROOT_DIR%\%PROJECT_NAME%\build-x0.5-dxt

if not exist %BUILD_DIR% (
    mkdir %BUILD_DIR%
)
cd %BUILD_DIR%

call %~dp0\assembly-assets-x0.5-dxt.bat
call python %MAZE_ENGINE_DIR%\..\tools\scripts\assets\copy_resources.py --src %ASSETS_X1_DXT_DIR% --dst %BUILD_DIR%\data
pause