@echo off

:: setup flags for cl.exe
set CompileFlags=/I"W:\deps\include" /I"W:\code\engine" /I"W:\code\game" /Od /Zi /w /MD /EHsc /Fm:mapfile /LD /DMOD_EXPORTS 

set LinkFlags=/NODEFAULTLIB:msvcrtd.lib /subsystem:console,5.01 w:\build\Engine.lib w:\build\Game.lib

set Files=mod.cpp

:: actually call cl
cl /Fe:mod.dll %CompileFlags% %Files% /link %LinkFlags%
