@echo off

:: create target directory if it doesnt exist
IF NOT EXIST W:\build mkdir w:\build
pushd w:\build

:: setup flags for cl.exe
set CompileFlags=/I"W:\deps\include" /Od /Zi /w /MD /EHsc 
set LinkFlags=/incremental:NO /NODEFAULTLIB:msvcrtd.lib /subsystem:console,5.01 W:\deps\lib\SDL2.lib W:\deps\lib\SDL2main.lib  W:\deps\lib\cyanlogger2.lib W:\deps\lib\SDL2_image.lib W:\deps\lib\SDL2_mixer.lib
set Files=w:\code\engine\main.cpp

:: actually call cl
cl /Fe:Engine.exe %CompileFlags% %Files% /link %LinkFlags%

:: go back to wherever we were
popd
