@echo off
cd %~dp0
call var.bat

set GRADLE_ANDROID="%~dp0\gradle-android"
call ..\..\..\templates\prj\win\configure-gradle-android.bat

pause