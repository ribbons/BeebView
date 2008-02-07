/**************************************************************************************************************
  Utility to display BBC Micro / Master Graphics Files.
  Copyright © 2008  www.nerdoftheherd.com
 
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

#define BV_WIDTH  640           // width  of "screen" (client area)
#define BV_DEFAULT_HEIGHT 512   // default height of "screen" (client area)

#define BBC_WIDTH0 640          // width of BBC MODE0 screen
#define BBC_WIDTH1 320          //       "      MODE1   "   
#define BBC_WIDTH2 160          //       "      MODE2   "   
#define BBC_WIDTH4 320          //       "      MODE4   "   
#define BBC_WIDTH5 160          //       "      MODE5   "    
#define BBC_XBLKS0 80			// Horizontal blocks on MODE 0 screen
#define BBC_XBLKS1 80			//           "          MODE 1   "   
#define BBC_XBLKS2 80			//           "          MODE 2   "   
#define BBC_XBLKS4 40			//           "          MODE 4   "   
#define BBC_XBLKS5 40			//           "          MODE 5   "   
#define BBC_DEFAULT_HEIGHT 256  // The default height of a BBC image
#define BYTES 8                 // number bytes in a block 

// Variables
HBITMAP TheBitmap;
char szFilterSpec [128] = "BBC Graphics Files (*.bbg)\0*.bbg\0All Files (*.*)\0*.*\0";
char szFileName[MAX_PATH] = "";
char szFileTitle[MAX_PATH] = "";
char szSaveFilterSpec [128] = "Windows Bitmap (*.bmp)\0*.bmp\0All Files (*.*)\0*.*\0";
char szSaveFileName[MAX_PATH] = "";
char szSaveFileTitle[MAX_PATH] = "";
char szWindowTitle[80];
int iClientHeight = BV_DEFAULT_HEIGHT;
int iBBCHeight = BBC_DEFAULT_HEIGHT;
bool bAutoSave = false;

// Message Handler Functions
BOOL BeebView_OnCreate(HWND hWnd, CREATESTRUCT FAR* lpCreateStruct);
void BeebView_OnInitMenuPopup(HWND hwnd, HMENU hMenu, UINT item, BOOL fSystemMenu);
void BeebView_OnCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify);
void BeebView_OnPaint(HWND hWnd);
void BeebView_OnDestroy(HWND hWnd);

// BeebView functions
void BeebView_UpdateTitle(HWND);
void BeebView_InitPalette(void);
COLORREF BeebView_GetColour(int);
void BeebView_SetMode(int);
void BeebView_CycleColour(int);
void BeebView_SetBitmapPixels(HWND);
int  BeebView_Width(HWND, int);
void BeebView_MakePic04(HWND, HANDLE, HDC, int);
void BeebView_MakePic15(HWND, HANDLE, HDC, int);
void BeebView_MakePic2(HWND, HANDLE, HDC, int);
void BeebView_SaveBitmap(HWND hWnd);

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