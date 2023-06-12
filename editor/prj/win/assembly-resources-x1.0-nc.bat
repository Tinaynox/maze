@echo off

call %~dp0\var.bat

set RESOURCES_X1_DIR=%PRJ_ROOT_DIR%\%PROJECT_NAME%\resources\resources-x1.0\data\
call python %MAZE_ENGINE_DIR%\..\tools\scripts\assets\assembly_resources.py  --src %RESOURCES_DIR% --dst %RESOURCES_X1_DIR% --tscale 1.0

set RESOURCES_X1_NC_DIR=%PRJ_ROOT_DIR%\%PROJECT_NAME%\resources\resources-x1.0-nc\data\
call python %MAZE_ENGINE_DIR%\..\tools\scripts\assets\make_data.py --src %RESOURCES_X1_DIR% --dst %RESOURCES_X1_NC_DIR% --tools %MAZE_ENGINE_DIR%\..\tools --temp %PRJ_ROOT_DIR%\%PROJECT_NAME%\resources\.temp --cache cache-win-x1.0-nc.pkl