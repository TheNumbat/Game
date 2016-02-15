@echo off

:: create target directory if it doesnt exist
IF NOT EXIST W:\build mkdir w:\build
pushd w:\build

:: setup flags for cl.exe
set CompileFlags=/I"W:\deps\include" /I"W:\code\engine" /Od /Zi /w /MD /EHsc /Fm:mapfile /LD /DGAME_EXPORTS
set LinkFlags=/incremental:NO /NODEFAULTLIB:msvcrtd.lib /subsystem:console,5.01 /PDB:Game_%random%.pdb w:\build\Engine.lib
set Files=w:\code\game\game.cpp w:\code\game\camera\camera.cpp

del Game_*.pdb > NUL 2> NUL

:: actually call cl
cl /Fe:Game.dll %CompileFlags% %Files% /link %LinkFlags%

:: go back to wherever we were
popd
