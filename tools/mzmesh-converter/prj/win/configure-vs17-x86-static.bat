@echo off
cd %~dp0
call var.bat


set CMAKELISTS_DIR=%~dp0..\..\
call %MAZE_ENGINE_DIR%\..\examples\templates\prj\win\configure-vs17-x86-static.bat

pause