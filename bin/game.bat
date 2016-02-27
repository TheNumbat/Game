@echo off

:: create target directory if it doesnt exist
IF NOT EXIST W:\build mkdir w:\build
pushd w:\build

:: setup flags for cl.exe
set CompileFlags=/I"W:\deps\include" /I"W:\code\engine" /I"W:\code\game" /Od /Zi /w /MD /EHsc /Fm:mapfile /LD /DGAME_EXPORTS

set LinkFlags=/NODEFAULTLIB:msvcrtd.lib /subsystem:console,5.01 /PDB:Game_%random%.pdb w:\build\Engine.lib

set Files=w:\code\game\game_main.cpp w:\code\game\camera\camera.cpp w:\code\game\event\event.cpp w:\code\game\map\mapMgr.cpp
set Files=%Files% w:\code\game\render\render.cpp w:\code\game\sim\sim.cpp w:\code\game\map\component\component.cpp
set Files=%Files% w:\code\game\map\position\position.cpp w:\code\game\map\entity\entity.cpp w:\code\game\map\chunk\chunk.cpp

del Game_*.pdb > NUL 2> NUL

:: actually call cl
cl /Fe:Game.dll %CompileFlags% %Files% /link %LinkFlags%

:: go back to wherever we were
popd
