@echo off
title Creating Zipped Release

rem make sure that 7za.exe is on the PATH.

cd Release
7za a -tzip "%userprofile%\Desktop\Beebview.zip" Beebview.exe
cd ..
7za a -tzip "%userprofile%\Desktop\Beebview.zip" GPL.txt
7za a -tzip "%userprofile%\Desktop\Beebview.zip" Examples\*.bbg