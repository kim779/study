del /f /s *.ncb
del /f /s *.obj
del /f /s *.bsc
del /f /s *.idb
del /f /s *.pdb
del /f /s *.pch
del /f /s *.ilk
del /f /s *.plg
del /f /s *.res
del /f /s *.sbr
del /f /s *.aps
del /f /s *.exp
del /f /s *.positions
del /f /s *.ast
del /f /s *.tmp
del /f /s *.scc
del /f /s *.stt
del /f /s *.opt
del /f /s *.clw
del /f /s *.bak
del /f /s *.ipch
del /f /s thumbs.db
FOR /F %%i in ('DIR /S /B /A:D RELEASE') DO RD /S /Q %%i
FOR /F %%i in ('DIR /S /B /A:D DEBUG' ) DO RD /S /Q %%i
FOR /F %%i in ('DIR /S /B /A:D Obj') DO RD /S /Q %%i
FOR /F %%i in ('DIR /S /B /A:D ObjD') DO RD /S /Q %%i
FOR /F %%i in ('DIR /S /B /A:D .vs') DO RD /S /Q %%i
FOR /F %%i in ('DIR /S /B /A:D ipch') DO RD /S /Q %%i

