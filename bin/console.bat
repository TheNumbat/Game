@echo off

:: create target directory if it doesnt exist
IF NOT EXIST W:\assets\console mkdir w:\assets\console
pushd w:\assets\console

:: setup flags for cl.exe
set CompileFlags=/I"W:\deps\include" /I"W:\code\engine" /I"W:\code\game" /Od /Zi /w /MD /EHsc /LD 

set LinkFlags=/NODEFAULTLIB:msvcrtd.lib /subsystem:console,5.01 w:\build\Engine.lib

set Files=w:\code\console\consoleFuncs.cpp

:: actually call cl
cl /Fe:console.dll %CompileFlags% %Files% /link %LinkFlags%

:: go back to wherever we were
popd
