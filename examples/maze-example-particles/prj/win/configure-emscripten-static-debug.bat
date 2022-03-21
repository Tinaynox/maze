@echo off
cd %~dp0
call var.bat


set CMAKELISTS_DIR=%~dp0..\..\
call ..\..\..\templates\prj\win\configure-emscripten-static-debug.bat

cd %~dp0
xcopy /y/q ..\..\sys-res\enscripten\index.html %BUILD_DIR%\
xcopy /y/q ..\..\sys-res\enscripten\emulate.bat %BUILD_DIR%\

pause