cls
@echo off
echo %~dp0
@echo on
echo creating dll build directory
mkdir IComModule_build\

echo entering build directory
cd IComModule_build\

echo qmake dll .pro file
C:\Qt\6.1.2\mingw81_64\bin\qmake.exe ..\com-module\IComModule\

echo making project
C:\Qt\Tools\mingw810_64\bin\mingw32-make.exe -j4

echo creating GUI build directory
cd ..
mkdir gui_build
cd gui_build

echo qmake GUI .pro file
C:\Qt\6.1.2\mingw81_64\bin\qmake.exe ..\IHM-module\

echo maing project
C:\Qt\Tools\mingw810_64\bin\mingw32-make.exe -j4

echo copying generated dll
copy ..\IComModule_build\release\IComModule.dll .\release

echo cleaning release directory
cd release
del *.o *.cpp *.h

echo deployment
C:\Qt\6.1.2\mingw81_64\bin\windeployqt.exe . --quick

echo exporting project
cd ..
move release ..\EthernetTestTool

echo clean all
cd ..\
rmdir /S /Q gui_build IComModule_build 

cd EthernetTestTool/
mkdir doc
mkdir TestReports

cd ..
Xcopy  /E/H/C/I "./IHM-module/doc" "./EthernetTestTool/doc"

pause