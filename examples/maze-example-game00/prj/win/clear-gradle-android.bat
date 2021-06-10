@echo off
cd %~dp0
call var.bat

set PRJ_DIR="%PRJ_ROOT_DIR%\%PROJECT_NAME%\prj.gradle-android"
@RD /s /q %PRJ_DIR%

pause