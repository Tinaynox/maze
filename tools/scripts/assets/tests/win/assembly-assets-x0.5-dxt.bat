@echo off

call %~dp0\var.bat

set ASSETS_X1_DIR=%PRJ_ROOT_DIR%\%PROJECT_NAME%\resources\resources-x0.5\data\
call python %MAZE_ENGINE_DIR%\..\tools\scripts\assets\assembly_resources.py  --src %ASSETS_DIR% --dst %ASSETS_X1_DIR% --tscales 0.5

set ASSETS_X1_DXT_DIR=%PRJ_ROOT_DIR%\%PROJECT_NAME%\resources\resources-x0.5-dxt\data\
call python %MAZE_ENGINE_DIR%\..\tools\scripts\assets\make_data.py --src %ASSETS_X1_DIR% --dst %ASSETS_X1_DXT_DIR% --tools %MAZE_ENGINE_DIR%\..\third-party\tools --temp %PRJ_ROOT_DIR%\%PROJECT_NAME%\resources\.temp --cache cache-win-x0.5-dxt.pkl --tcompression dxt --tcompression-ext-policy 1