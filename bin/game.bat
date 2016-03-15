@echo off

:: create target directory if it doesnt exist
IF NOT EXIST W:\build mkdir w:\build
pushd w:\build

:: setup flags for cl.exe
set CompileFlags=/I"W:\deps\include" /I"W:\code\engine" /I"W:\code\game" /Od /Zi /w /MD /EHsc /Fm:mapfile /LD /DGAME_EXPORTS 

set LinkFlags=/NODEFAULTLIB:msvcrtd.lib /subsystem:console,5.01 /PDB:Game_%random%.pdb w:\build\Engine.lib

set Files=w:\code\game\dll_interface.cpp w:\code\game\game_state.cpp w:\code\game\cam\cam.cpp w:\code\game\debug\debugMgr.cpp
set Files=%Files% w:\code\game\input\inputMgr.cpp w:\code\game\map\mapMgr.cpp w:\code\game\map\chunk\chunk.cpp
set Files=%Files% w:\code\game\map\component\component.cpp w:\code\game\map\entity\entity.cpp w:\code\game\map\position\position.cpp
set Files=%Files% w:\code\game\render\renderMgr.cpp w:\code\game\mod\modMgr.cpp

del Game_*.pdb > NUL 2> NUL

:: actually call cl
cl /Fe:Game.dll %CompileFlags% %Files% /link %LinkFlags%

:: go back to wherever we were
popd

call console.bat
