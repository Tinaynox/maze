@echo off
cd %~dp0
call var.bat


set CMAKELISTS_DIR=%~dp0..\..\
call ..\..\..\templates\prj\win\configure-emscripten-static-release.bat

cd %~dp0
xcopy /y/q ..\..\sys-res\enscripten\%PROJECT_NAME%.html %BUILD_DIR%\
xcopy /y/q ..\..\sys-res\enscripten\emulate.bat %BUILD_DIR%\

pause