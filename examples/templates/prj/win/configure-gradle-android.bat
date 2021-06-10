@echo off

set PRJ_DIR="%PRJ_ROOT_DIR%\%PROJECT_NAME%\prj.gradle-android"


if not exist %PRJ_DIR% (
    mkdir %PRJ_DIR%
)

call %~dp0\assembly-resources-x1.0-nc.bat

xcopy /s/y/e/q ..\..\..\templates\prj\win\android\gradle-android.template %PRJ_DIR%\
xcopy /s/y/e/q %GRADLE_ANDROID% %PRJ_DIR%\