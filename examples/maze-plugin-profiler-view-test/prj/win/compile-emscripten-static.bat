@echo off
cd %~dp0
call var.bat

call ..\..\..\templates\prj\win\compile-emscripten-static.bat

pause