@echo off

rem Batch file to build, sign and zip up Beebview

set sdklocation=%programfiles%\Microsoft SDKs\Windows\v7.0
if not exist "%sdklocation%" goto nosdk

set timestampserver=http://timestamp.verisign.com/scripts/timstamp.dll
set tempfolder=%temp%\BeebviewPub

rem Required to run the SDK setenv script
setlocal ENABLEEXTENSIONS
setlocal ENABLEDELAYEDEXPANSION

rem Set up a Release build environment
call "%sdklocation%\Bin\setenv.cmd" /Release
if ERRORLEVEL 1 goto failed

rem Clean and build Win32 Beebview

msbuild /p:Configuration=Release /p:Platform=Win32 /t:Clean
if ERRORLEVEL 1 goto failed

msbuild /p:Configuration=Release /p:Platform=Win32
if ERRORLEVEL 1 goto failed

rem Clean and build x64 Beebview

msbuild /p:Configuration=Release /p:Platform=x64 /t:Clean
if ERRORLEVEL 1 goto failed

msbuild /p:Configuration=Release /p:Platform=x64
if ERRORLEVEL 1 goto failed

rem Sign Beebview

signtool sign /t %timestampserver% "bin\Win32\Release\Beebview.exe" "bin\x64\Release\Beebview.exe"
if ERRORLEVEL 1 set signfailed=1

rem Zip up the files to release
rem Make sure that 7za.exe is on the PATH.

rem create the temp folder if it doesn't already exist
if not exist "%tempfolder%" mkdir "%tempfolder%"
if ERRORLEVEL 1 goto failed

if not exist "%tempfolder%\Win32" mkdir "%tempfolder%\Win32"
if ERRORLEVEL 1 goto failed

if not exist "%tempfolder%\x64" mkdir "%tempfolder%\x64"
if ERRORLEVEL 1 goto failed

copy bin\Win32\Release\Beebview.exe "%tempfolder%\Win32\"
if ERRORLEVEL 1 goto failed

copy bin\x64\Release\Beebview.exe "%tempfolder%\x64\"
if ERRORLEVEL 1 goto failed

7za a -tzip "%userprofile%\Desktop\Beebview.zip" "%tempfolder%\*"
if ERRORLEVEL 1 goto failed

rmdir /S /Q "%tempfolder%"
if ERRORLEVEL 1 goto failed

7za a -tzip "%userprofile%\Desktop\Beebview.zip" GPL.txt
7za a -tzip "%userprofile%\Desktop\Beebview.zip" Examples\*.bbg

if not "%signfailed%" == "" goto signfailed

goto exit

:nosdk

echo The Microsoft Windows SDK for Windows 7 does not appear to be installed
echo Please install it and then try running this script again

pause

goto exit

:signfailed

echo.
echo Warning: Failed to sign one or more of the binaries or installer
echo Check that you have a code signing certificate installed and try again.

pause

goto :EOF

:failed

echo.
echo Publish failed - review above output for more details

pause
