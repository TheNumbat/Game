@echo off

:: create target directory if it doesnt exist
IF NOT EXIST W:\build mkdir w:\build
pushd w:\build

:: setup flags for cl.exe
set CompileFlags=/I"W:\deps\include" /I"W:\code\include" /Od /Zi /w /MD /EHsc 
set LinkFlags=/incremental:NO /NODEFAULTLIB:msvcrtd.lib /subsystem:console,5.01 W:\deps\lib\SDL2.lib W:\deps\lib\SDL2main.lib  W:\deps\lib\cyanlogger2.lib W:\deps\lib\SDL2_image.lib W:\deps\lib\SDL2_mixer.lib

set Files=w:\code\engine\main.cpp w:\code\engine\event\event\event.cpp w:\code\engine\event\eventMgr.cpp w:\code\engine\graphic\camera\camera.cpp w:\code\engine\graphic\texture\texture.cpp
set Files=%Files% w:\code\engine\graphic\graphicMgr.cpp w:\code\engine\log\logMgr.cpp w:\code\engine\map\chunk\chunk.cpp w:\code\engine\map\component\component.cpp w:\code\engine\map\entity\entity.cpp
set Files=%Files% w:\code\engine\map\position\position.cpp w:\code\engine\map\mapMgr.cpp w:\code\engine\sound\sound\sound.cpp w:\code\engine\sound\soundMgr.cpp w:\code\engine\state\state.cpp
set Files=%Files% w:\code\engine\thread\threadMgr.cpp w:\code\engine\time\timer\timer.cpp w:\code\engine\time\timeMgr.cpp 

:: actually call cl
cl /Fe:Engine.exe %CompileFlags% %Files% /link %LinkFlags%

:: go back to wherever we were
popd
