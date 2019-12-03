@REM Build Engine, Compiler and IDE for Windows
@REM also copy dependencies.
@echo off
call vcvars32

REM Clean bin
rd /S /Q bin
md bin

REM Compile Engine
msbuild ..\..\Source\Engine\platform\windows\SwiftEngine.sln^
 /p:Configuration=Release^
 /p:Platform="Win32"^
 /t:Rebuild

REM Copy Engine
copy ..\..\Source\Engine\bin\windows\Release\SwiftEngine.exe^
 bin\SwiftEngine.exe

REM Compile Compiler
msbuild ..\..\Source\Compiler\project\windows\Compiler.sln^
 /p:Configuration=Release^
 /p:Platform="Any CPU"^
 /t:Rebuild

REM Copy Compiler
copy ..\..\Source\Compiler\bin\windows\Release\Swift.exe^
 bin\Swift.exe

copy ..\..\Source\Compiler\bin\windows\Release\Swift.exe.config^
 bin\Swift.exe.config

REM Compile IDE
pushd ..\..\Source\IDE\bin\Release\
nmake all
popd

REM Copy IDE
copy ..\..\Source\IDE\bin\Release\release\IDE.exe^
 bin\IDE.exe

REM Copy dependencies
xcopy deps bin /e
