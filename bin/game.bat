@echo off

:: create target directory if it doesnt exist
IF NOT EXIST W:\build mkdir w:\build
pushd w:\build

:: setup flags for cl.exe
set CompileFlags=/I"W:\deps\include" /I"W:\code\include" /Od /Zi /w /MD /EHsc /Fm:mapfile /LD
set LinkFlags=/incremental:NO /NODEFAULTLIB:msvcrtd.lib /subsystem:console,5.01 /PDB:Game_%random%.pdb
set Files=w:\code\game\game.cpp

del Game_*.pdb > NUL 2> NUL

:: actually call cl
cl /Fe:Game.dll %CompileFlags% %Files% /link %LinkFlags%

:: go back to wherever we were
popd
