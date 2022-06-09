taskkill /im qtcreator.exe
timeout 1
copy /y ..\release\shared\SFBWidgets.dll C:\Development\Qt\Qt5.2.1-32\Tools\QtCreator\bin\plugins\designer\
C:\Development\Qt\Qt5.2.1-32\Tools\QtCreator\bin\qtcreator.exe ..\SFBWidgets.pro
