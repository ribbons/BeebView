/**************************************************************************************************************
  Utility to display BBC Micro / Master Graphics Files.
  Copyright © 2008 Matt Robinson, © 2003 David Robinson
 
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

#include "resource.h"

#define READBUF 100
#define LOADFILTER "BBC Graphics Files (*.bbg)\0*.bbg\0All Files (*.*)\0*.*\0"
#define SAVEFILTER "Windows Bitmap (*.bmp)\0*.bmp\0All Files (*.*)\0*.*\0"

// Application global variables
extern HINSTANCE hInst; // current instance

// Standard Windows application functions
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

// Message Handler functions
BOOL BeebView_OnCreate(HWND hWnd, CREATESTRUCT FAR* lpCreateStruct);
void BeebView_OnInitMenuPopup(HWND hwnd, HMENU hMenu, UINT item, BOOL fSystemMenu);
void BeebView_OnCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify);
void BeebView_OnPaint(HWND hWnd);
void BeebView_OnDestroy(HWND hWnd);

// BeebView functions
void BeebView_UpdateTitle(HWND);
void BeebView_CycleColour(int);
void BeebView_OpenFile(HWND hWnd);
void BeebView_LoadFile(HWND hWnd, char *fileName);
void BeebView_ForceRepaint(HWND hWnd);
BOOL BeebView_LoadMemDump(HWND hWnd, char *fileName);
void BeebView_SaveBitmapPrompt(HWND hWnd);
void BeebView_SaveBitmap(HWND hWnd, char *saveFileName);
int dispHeight(int bbcHeight);

// Util functions
BOOL CenterWindow (HWND hwndChild, HWND hwndParent);
int WindowHeight(int iClientHeight);
int WindowWidth();
void deleteExtension(char *fileName);
