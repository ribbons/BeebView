/*
 * This file is part of BBC Graphics Viewer.
 * Copyright © 2003-2010 by the authors - see the AUTHORS file for details.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "resource.h"

#define BV_WIDTH          640    // width  of "screen" (client area)
#define BV_DEFAULT_HEIGHT 512    // default height of "screen" (client area)
#define BV_READBUF        1024   // Buffer size for loading memory dump files
#define BV_MEMSIZE012     20480  // Screen memory size for modes 0, 1 & 2
#define BV_MEMSIZE45      10240  // Screen memory size for modes 4 & 5

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
void BeebView_CycleColour(unsigned char colour);
void BeebView_OpenFile(HWND hWnd);
void BeebView_LoadFile(HWND hWnd, char *fileName);
void BeebView_ForceRepaint(HWND hWnd);
void BeebView_LoadMemDump(HANDLE hFileHandle);
bool BeebView_LoadLdPic(HANDLE hFileHandle);
void BeebView_SaveBitmapPrompt(HWND hWnd);
void BeebView_SaveBitmap(HWND hWnd, char *saveFileName);

// Util functions
BOOL CenterWindow (HWND hwndChild, HWND hwndParent);
int  WindowHeight(int iClientHeight);
int  WindowWidth();
int  dispHeight(int bbcHeight);
void deleteExtension(char *fileName);
bool getBitsFromFile(HANDLE hFileHandle, int numBits, bool flushStore, unsigned char *fileBits);
bool getBitFromFile(HANDLE hFileHandle, bool flushStore, unsigned char *fileBit);
