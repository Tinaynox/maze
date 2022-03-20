@echo off
cd %~dp0
call var.bat


set CMAKELISTS_DIR=%~dp0..\..\
call ..\..\..\templates\prj\win\configure-vs19-x64-static.bat

pause