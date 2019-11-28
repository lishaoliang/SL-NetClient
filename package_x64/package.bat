@echo off

xcopy /y E:\Qt_project\SLNetClient\release_x64\release\IPCtest_x64.exe E:\Qt_project\SLNetClient\package_x64\packages\org.qtproject.ifw.example\data /V 
echo copysuccess
xcopy /y E:\Qt_project\SLNetClient\lib_x64\*.dll E:\Qt_project\SLNetClient\package_x64\packages\org.qtproject.ifw.example\data
ping -n 3 127.0.0.1>nul
call D:\Qt\Qt5.9.2\5.9.2\msvc2015_64\bin\qtenv2.bat
windeployqt E:\Qt_project\SLNetClient\package_x64\packages\org.qtproject.ifw.example\data\IPCtest_x64.exe

cd /d E:\Qt_project\SLNetClient\package_x64
binarycreator.exe -c config/config.xml -p packages SLNetClient_install_x64.exe -v
