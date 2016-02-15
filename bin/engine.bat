@echo off

:: create target directory if it doesnt exist
IF NOT EXIST W:\build mkdir w:\build
pushd w:\build

:: setup flags for cl.exe
set CompileFlags=/I"W:\deps\include" /I"W:\code\include" /I"W:\code\engine" /I"W:\code\game" /Od /Zi /w /MD /EHsc /DENGINE_EXPORTS
set LinkFlags=/incremental:NO /NODEFAULTLIB:msvcrtd.lib /subsystem:console,5.01 W:\deps\lib\SDL2.lib W:\deps\lib\SDL2main.lib  W:\deps\lib\cyanlogger2.lib W:\deps\lib\SDL2_image.lib W:\deps\lib\SDL2_mixer.lib

set Files=w:\code\engine\event\event\event.cpp w:\code\engine\event\eventMgr.cpp w:\code\engine\graphic\texture\texture.cpp w:\code\engine\map\entity\entity.cpp 
set Files=%Files% w:\code\engine\graphic\graphicMgr.cpp w:\code\engine\log\logMgr.cpp w:\code\engine\map\component\component.cpp w:\code\engine\sdl\sdlMgr.cpp
set Files=%Files% w:\code\engine\map\position\position.cpp w:\code\engine\map\mapMgr.cpp w:\code\engine\sound\sound\sound.cpp w:\code\engine\sound\soundMgr.cpp
set Files=%Files% w:\code\engine\thread\threadMgr.cpp w:\code\engine\time\timer\timer.cpp w:\code\engine\time\timeMgr.cpp 

:: actually call cl
cl /Fe:Engine.dll /LD %CompileFlags% %Files% /link %LinkFlags%
cl /Fe:Run.exe %CompileFlags% w:\code\engine\main.cpp /link %LinkFlags% w:\build\Engine.lib

:: go back to wherever we were
popd
