/**************************************************************************************************************
  Utility to display BBC Micro / Master Graphics Files.
  Copyright © 2007-2010 Matt Robinson, © 2003 David Robinson
 
  This program is free software; you can redistribute it and/or modify it under the terms of the GNU General
  Public License as published by the Free Software Foundation; either version 2 of the License, or (at your
  option) any later version.
 
  This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
  License for more details.
 
  You should have received a copy of the GNU General Public License along with this program; if not, write
  to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
**************************************************************************************************************/

#pragma once

#define STRICT                  // Enable strict type checking
#define WIN32_LEAN_AND_MEAN     // Exclude rarely-used stuff from Windows headers

#include <windows.h>            // Windows Headers
#include <windowsx.h>           // Strict macro and message crackers etc
#include <shellapi.h>           // For ShellExecute etc
#include <commdlg.h>            // Common Dialogs

#include <stdlib.h>
#include <malloc.h>
#include <tchar.h>
#include <assert.h>
#include <stdio.h>
#include <stdexcept>
