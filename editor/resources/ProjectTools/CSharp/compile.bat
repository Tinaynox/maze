msbuild prj/Assembly-CSharp.csproj /p:Configuration=Release
xcopy /s/y/e/q bin\Release\*.dll ..\Library\ScriptAssemblies
