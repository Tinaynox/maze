@echo off

call %~dp0\var.bat

set ASSETS_X1_DIR=%PRJ_ROOT_DIR%\%PROJECT_NAME%\resources\resources-x1.0\data\
call python %MAZE_ENGINE_DIR%\..\tools\scripts\assets\assembly_resources.py  --src %ASSETS_DIR% --dst %ASSETS_X1_DIR% --tscale 1.0

set ASSETS_X1_NC_DIR=%PRJ_ROOT_DIR%\%PROJECT_NAME%\resources\resources-x1.0-nc\data\
call python %MAZE_ENGINE_DIR%\..\tools\scripts\assets\make_data.py --src %ASSETS_X1_DIR% --dst %ASSETS_X1_NC_DIR% --tools %MAZE_ENGINE_DIR%\..\third-party\tools --temp %PRJ_ROOT_DIR%\%PROJECT_NAME%\resources\.temp --cache cache-win-x1.0-nc.pkl