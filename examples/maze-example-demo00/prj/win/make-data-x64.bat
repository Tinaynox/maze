@echo off
cd %~dp0
call var.bat


set RESOURCES_DIR=%~dp0..\..\resources\data
call ..\..\..\templates\prj\win\make-data-x64.bat

pause