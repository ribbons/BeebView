@echo off

rem This file is part of BBC Graphics Viewer.
rem Copyright © 2016 by the authors - see the AUTHORS file for details.
rem
rem This program is free software: you can redistribute it and/or modify
rem it under the terms of the GNU General Public License as published by
rem the Free Software Foundation, either version 3 of the License, or
rem (at your option) any later version.
rem
rem This program is distributed in the hope that it will be useful,
rem but WITHOUT ANY WARRANTY; without even the implied warranty of
rem MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
rem GNU General Public License for more details.
rem
rem You should have received a copy of the GNU General Public License
rem along with this program.  If not, see <http://www.gnu.org/licenses/>.

if "%~1"=="" (
    echo Please specify the path to the Qt installation
    exit /B 1
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
cmake --build . --target package --config Release || exit /B 1

copy "%BUILDDIR%\*.msi" "%~dp0" || exit /B 1

endlocal
rmdir /s /q "%BUILDDIR%" || exit /B 1

goto :EOF
