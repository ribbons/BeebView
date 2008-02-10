@echo off
title Creating Zipped Source

set tempfolder="%temp%\Beebview"

rem create the temp folder if it doesn't already exist
if not exist %tempfolder% mkdir %tempfolder%

rem copy all of the files across that are needed, but try and miss out the files that can be easily re-created.
robocopy .\ %tempfolder% /MIR /XD .svn Debug Release /XF Beebview.ncb Beebview.vcproj.*.*.user Beebview.suo

rem make sure that 7za.exe is on the PATH.
7za a -tzip "%userprofile%\Desktop\Beebview Source.zip" %tempfolder%

rmdir /S /Q %tempfolder%