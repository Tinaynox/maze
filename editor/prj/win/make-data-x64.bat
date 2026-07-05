@echo off
cd %~dp0
call var.bat

set RESOURCES_DIR=%~dp0..\..\resources
set BUILD_DIR=%PRJ_ROOT_DIR%\%PROJECT_NAME%\build.x64

if not exist %BUILD_DIR% (mkdir %BUILD_DIR%)
cd %BUILD_DIR%


set CSHARP_LIB=%MAZE_ENGINE_DIR%\..\plugins\csharp\bin\x64\Debug
if not exist %BUILD_DIR% (mkdir %CSHARP_LIB_DIR%)
xcopy /s/y/e/q %CSHARP_LIB% %RESOURCES_DIR%\Libs

rem Copy assets assembly tools
xcopy /s/y/e/q/i %MAZE_ENGINE_DIR%\..\tools\scripts\assets %BUILD_DIR%\tools\scripts\assets
xcopy /s/y/e/q/i %MAZE_ENGINE_DIR%\..\tools\mzdata-converter\bin %BUILD_DIR%\tools\mzdata-converter\bin
xcopy /s/y/e/q/i %MAZE_ENGINE_DIR%\..\third-party\tools\TheCompressonator %BUILD_DIR%\third-party\tools\TheCompressonator
xcopy /s/y/e/q/i %MAZE_ENGINE_DIR%\..\third-party\tools\Cheetah-Texture-Packer\bin\win %BUILD_DIR%\third-party\tools\Cheetah-Texture-Packer\bin\win
xcopy /s/y/e/q/i %MAZE_ENGINE_DIR%\..\third-party\tools\PVRTexTool\CLI\Windows_x86_32 %BUILD_DIR%\third-party\tools\PVRTexTool\CLI\Windows_x86_32

call %~dp0\assembly-resources-x1.0-nc.bat
call python %MAZE_ENGINE_DIR%\..\tools\scripts\assets\copy_resources.py --src %RESOURCES_X1_NC_DIR% --dst %BUILD_DIR%\data
xcopy /s/y/e/q  %EXAMPLES_LIB_DIR%\x64 %BUILD_DIR%

pause