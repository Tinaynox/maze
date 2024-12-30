@echo off
cd %~dp0
call var.bat

set RESOURCES_DIR=%~dp0..\..\resources
set BUILD_DIR=%PRJ_ROOT_DIR%\%PROJECT_NAME%\build.x86

if not exist %BUILD_DIR% (mkdir %BUILD_DIR%)
cd %BUILD_DIR%


set CSHARP_LIB=%MAZE_ENGINE_DIR%\..\plugins\csharp\bin\x86\Debug
if not exist %BUILD_DIR% (mkdir %CSHARP_LIB_DIR%)
xcopy /s/y/e/q %CSHARP_LIB% %RESOURCES_DIR%\Libs

call %~dp0\assembly-resources-x1.0-nc.bat
call python %MAZE_ENGINE_DIR%\..\tools\scripts\assets\copy_resources.py --src %RESOURCES_X1_NC_DIR% --dst %BUILD_DIR%\data
xcopy /s/y/e/q  %EXAMPLES_LIB_DIR%\x86 %BUILD_DIR%

pause