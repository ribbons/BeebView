@echo off

rem Copyright © 2016 Matt Robinson
rem
rem SPDX-License-Identifier: GPL-3.0-or-later

setlocal

if "%~1"=="" (
    echo Please specify the path to the Qt installation
    exit /B 1
)

if not "%~2"=="unsigned" (
    set SUFFIX=_signed
)

set PREFIX_PATH_32=%~1\msvc2015
set PREFIX_PATH_64=%~1\msvc2015_64

if not exist "%PREFIX_PATH_32%" (
    echo Can't find folder "%PREFIX_PATH_32%"
    exit /B 1
)

if not exist "%PREFIX_PATH_64%" (
    echo Can't find folder "%PREFIX_PATH_64%"
    exit /B 1
)

call :BUILD "%PREFIX_PATH_32%" "Visual Studio 14 2015" || exit /B 1
call :BUILD "%PREFIX_PATH_64%" "Visual Studio 14 2015 Win64" || exit /B 1

goto :EOF

:BUILD
:NEXTNAME
set BUILDDIR=%~dp0\..\build\%RANDOM%
if exist "%BUILDDIR%" GOTO :NEXTNAME

setlocal ENABLEEXTENSIONS
mkdir "%BUILDDIR%" || exit /B 1
cd /d "%BUILDDIR%" || exit /B 1

cmake -DCMAKE_PREFIX_PATH="%~1" -G"%~2" "%~dp0\.." || exit /B 1

cmake --build . --target beebview%SUFFIX% --config Release || exit /B 1
cmake --build . --target package%SUFFIX% --config Release || exit /B 1

copy "%BUILDDIR%\*.msi" "%~dp0" || exit /B 1
copy "%BUILDDIR%\*.zip" "%~dp0" || exit /B 1

endlocal
rmdir /s /q "%BUILDDIR%" || exit /B 1

goto :EOF
