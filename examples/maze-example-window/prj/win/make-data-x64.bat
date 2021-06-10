@echo off
cd %~dp0
call var.bat


set RESOURCES_DIR=%~dp0..\..\..\resources
echo RESOURCES_DIR=%RESOURCES_DIR%
call ..\..\..\templates\prj\win\make-data-x64.bat

pause