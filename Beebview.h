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

// Message Handler Functions
BOOL BeebView_OnCreate(HWND hWnd, CREATESTRUCT FAR* lpCreateStruct);
void BeebView_OnInitMenuPopup(HWND hwnd, HMENU hMenu, UINT item, BOOL fSystemMenu);
void BeebView_OnCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify);
void BeebView_OnPaint(HWND hWnd);
void BeebView_OnDestroy(HWND hWnd);

// BeebView functions
void BeebView_UpdateTitle(HWND);
COLORREF BeebView_GetColour(int);
void BeebView_CycleColour(int);
void BeebView_LoadFile(HWND hWnd);
void BeebView_LoadMemDump(HWND hWnd);
void BeebView_SaveBitmap(HWND hWnd);
int dispHeight(int bbcHeight);

// Util Functions
BOOL SaveDialog(HWND hwndOwner, LPSTR filter, LPSTR fil, UINT iFilLen, LPSTR dlgtitle, LPSTR filtitle, UINT iFilTitleLen, LPSTR lpstrDefExt);
BOOL OpenDialog(HWND hwndOwner, LPSTR filter, LPSTR fil, UINT iFilLen, LPSTR dlgtitle, LPSTR filtitle, UINT iFilTitleLen);
BOOL CenterWindow (HWND hwndChild, HWND hwndParent);
int WindowHeight(int iClientHeight);
int WindowWidth();
void SaveDib(HDC hDC, LPCTSTR lpszFileName, BOOL bOverwriteExisting);


namespace dib
{
    int GetBytesPerPixel(int depth);
    int GetBytesPerRow(int width, int depth);
    int GetBitmapBytes(int width, int height, int depth);
    int GetBitmapBytes(const BITMAPINFOHEADER *bmih);
};