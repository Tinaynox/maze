@echo off
cd %~dp0
call var.bat


set CMAKELISTS_DIR=%~dp0..\..\
call ..\..\..\templates\prj\win\configure-vs17-arm64-static.bat

pause