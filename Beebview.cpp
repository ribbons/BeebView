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

#include "stdafx.h"
#include "Beebview.h"
#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szAppName[MAX_LOADSTRING];			    // The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

int nMode;
int pal_bit[16];
COLORREF palette[16];

OFSTRUCT of;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
 	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szAppName, MAX_LOADSTRING);
	LoadString(hInstance, IDC_BEEBVIEW, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Set up the palette and default mode
	BeebView_InitPalette();
	BeebView_SetMode(1);

	// Work through the command line parameters.
	// This block of code will set szFileName to the last quoted part of the command line, and pick up any other params.
	char *params;
	char *context;
	bool bInQuotes = false;

	params = strtok_s(lpCmdLine, " ", &context);

	while (params != NULL)
	{
		if(bInQuotes) {
			strcat_s(szFileName, " ");
			strcat_s(szFileName, params);
			if(params[strlen(params)-1] == '"') {
				strncpy_s(szFileName, MAX_PATH, szFileName, strlen(szFileName)-1);
				bInQuotes = false;
			}
		} else {
			if(params[0] == '"') {
				strcpy_s(szFileName, params+1);
				if(params[strlen(params)-1] == '"') {
					strncpy_s(szFileName, MAX_PATH, szFileName, strlen(szFileName)-1);
				} else {
					bInQuotes = true;
				}
			} else {
				// Process other command line params
				if(strcmp(params, "--save") == 0 ) {
					bAutoSave = true;
				} else if(strcmp(params, "--mode0") == 0 ) {
					BeebView_SetMode(0);
				} else if(strcmp(params, "--mode1") == 0 ) {
					BeebView_SetMode(1);
				} else if(strcmp(params, "--mode2") == 0 ) {
					BeebView_SetMode(2);
				} else if(strcmp(params, "--mode4") == 0 ) {
					BeebView_SetMode(4);
				} else if(strcmp(params, "--mode5") == 0 ) {
					BeebView_SetMode(5);
				}
			}
		}

		params = strtok_s(NULL, " ", &context);
	}

	// If szFileName has been set, set szFileTitle to the file title (without extension).
	if(strlen(szFileName) > 0) {
		char *tempTitle = strrchr(szFileName, '\\') + 1;
		strcpy_s(szFileTitle, tempTitle);
		char *pPos = strrchr(szFileTitle, '.');

		if(pPos != NULL) {
			strncpy_s(szFileTitle, MAX_PATH, szFileTitle, (int)(pPos - szFileTitle));
		}
	}

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_BEEBVIEW);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_BEEBVIEW);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= GetStockBrush(BLACK_BRUSH);
	wcex.lpszMenuName	= (LPCTSTR)IDC_BEEBVIEW;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szAppName, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
      CW_USEDEFAULT, 0, WindowWidth(), WindowHeight(BV_DEFAULT_HEIGHT), NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) 
	{
		HANDLE_MSG(hWnd, WM_CREATE,        BeebView_OnCreate);
		HANDLE_MSG(hWnd, WM_INITMENUPOPUP, BeebView_OnInitMenuPopup);
		HANDLE_MSG(hWnd, WM_COMMAND,       BeebView_OnCommand);
		HANDLE_MSG(hWnd, WM_PAINT,         BeebView_OnPaint);
		HANDLE_MSG(hWnd, WM_DESTROY,       BeebView_OnDestroy);
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		CenterWindow (hDlg, GetWindow (hDlg, GW_OWNER)); 
		
		// Set the license text
		TCHAR lpLicense[1000];
		LoadString(hInst, IDS_LICENSETEXT, lpLicense, sizeof(lpLicense)/sizeof(lpLicense[0]));
		SetDlgItemText (hDlg, IDC_LICENSE, lpLicense);
		
		// Calculate the height of font that we need to use
		HDC hdc;
		long lfHeight;
		hdc = GetDC(NULL);
		lfHeight = -MulDiv(8, GetDeviceCaps(hdc, LOGPIXELSY), 72);
		ReleaseDC(NULL, hdc);
		
		// Set the font for the url style label (to give us underline)
		SendMessage (GetDlgItem (hDlg, IDC_URL), WM_SETFONT, (WPARAM)CreateFont(lfHeight, 0, 0, 0, 0, FALSE, TRUE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "MS Shell Dlg"), FALSE);
		
		// Set the cursor for the url style label
		HCURSOR hCursor;
		hCursor = LoadCursor(NULL, IDC_HAND);
		SetClassLong(GetDlgItem (hDlg, IDC_URL), GCL_HCURSOR, (LONG)hCursor);
		return TRUE;
		
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		if (LOWORD(wParam) == IDC_URL)
		{
			ShellExecute(NULL, "open", "http://www.nerdoftheherd.com/tools/beebview/", NULL, NULL, SW_SHOWNORMAL);
			return TRUE;
		}
		break;

	case WM_CTLCOLORSTATIC:
		if((HWND)lParam == GetDlgItem(hDlg, IDC_URL))
		{
			SetTextColor((HDC)wParam, RGB(0, 0, 255));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (BOOL)GetStockObject(NULL_BRUSH);
		}
		break;
	}
	return FALSE;
}

// Message Handler Functions -----------------------------------------------------------------------

BOOL BeebView_OnCreate(HWND hWnd, CREATESTRUCT FAR* lpCreateStruct)
{
	BeebView_SetBitmapPixels(hWnd);

	// Automatically save the file and exit if bAutoSave is true
	if(bAutoSave) {
		// Add .bmp to the loaded file.
		strcpy_s(szSaveFileName, szFileName);
		strcat_s(szSaveFileName, ".bmp");
		BeebView_SaveBitmap(hWnd);

		// Close the program
		DestroyWindow(hWnd);
	}

	return TRUE;
}

void BeebView_OnInitMenuPopup(HWND hwnd, HMENU hMenu, UINT item, BOOL fSystemMenu)
{
	// Disable Save As... if no image is loaded
	if(strlen(szFileName) > 0) {
		EnableMenuItem(hMenu, IDM_SAVEAS, MF_ENABLED);
	} else {
		EnableMenuItem(hMenu, IDM_SAVEAS, MF_GRAYED);
	}

	switch(nMode) {
		case 0:
		case 4:
			EnableMenuItem(hMenu, IDM_COL0, MF_ENABLED);
			EnableMenuItem(hMenu, IDM_COL1, MF_DISABLED);
			EnableMenuItem(hMenu, IDM_COL2, MF_DISABLED);
			EnableMenuItem(hMenu, IDM_COL3, MF_DISABLED);
			EnableMenuItem(hMenu, IDM_COL4, MF_DISABLED);
			EnableMenuItem(hMenu, IDM_COL5, MF_DISABLED);
			EnableMenuItem(hMenu, IDM_COL6, MF_DISABLED);
			EnableMenuItem(hMenu, IDM_COL7, MF_ENABLED);
			break;
		case 1:
		case 5:
			EnableMenuItem(hMenu, IDM_COL0, MF_ENABLED);
			EnableMenuItem(hMenu, IDM_COL1, MF_ENABLED);
			EnableMenuItem(hMenu, IDM_COL2, MF_ENABLED);
			EnableMenuItem(hMenu, IDM_COL3, MF_ENABLED);
			EnableMenuItem(hMenu, IDM_COL4, MF_DISABLED);
			EnableMenuItem(hMenu, IDM_COL5, MF_DISABLED);
			EnableMenuItem(hMenu, IDM_COL6, MF_DISABLED);
			EnableMenuItem(hMenu, IDM_COL7, MF_DISABLED);
			break;
		case 2:
			EnableMenuItem(hMenu, IDM_COL0, MF_ENABLED);
			EnableMenuItem(hMenu, IDM_COL1, MF_ENABLED);
			EnableMenuItem(hMenu, IDM_COL2, MF_ENABLED);
			EnableMenuItem(hMenu, IDM_COL3, MF_ENABLED);
			EnableMenuItem(hMenu, IDM_COL4, MF_ENABLED);
			EnableMenuItem(hMenu, IDM_COL5, MF_ENABLED);
			EnableMenuItem(hMenu, IDM_COL6, MF_ENABLED);
			EnableMenuItem(hMenu, IDM_COL7, MF_ENABLED);
			break;
	}
	
	// Set a radio check next to the menu item for the current mode.
	UINT iCheckMode;
	
	switch(nMode) {
		case 0:
			iCheckMode=IDM_MODE0;
			break;
		case 1:
			iCheckMode=IDM_MODE1;
			break;
		case 2:
			iCheckMode=IDM_MODE2;
			break;
		case 4:
			iCheckMode=IDM_MODE4;
			break;
		case 5:
			iCheckMode=IDM_MODE5;
			break;
	}
	
	CheckMenuRadioItem(hMenu, IDM_MODE0, IDM_MODE5, iCheckMode, MF_BYCOMMAND);
}

void BeebView_OnCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify)
{
	// Parse the menu selections:
	switch (id)
	{
		case IDM_OPEN:
			OpenDialog(hWnd, szFilterSpec, szFileName, MAX_PATH, "Open file", szFileTitle, MAX_PATH);
			BeebView_SetBitmapPixels(hWnd);
			break;
		case IDM_SAVEAS:
			// Set the default file title
			strcpy_s(szSaveFileName, szFileTitle);
			// Show the save as dialog
			if(SaveDialog(hWnd, szSaveFilterSpec, szSaveFileName, MAX_PATH, "Save As", szSaveFileTitle, MAX_PATH, "bmp")) {
				BeebView_SaveBitmap(hWnd);
			}
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case IDM_MODE0:
			BeebView_SetMode(0);
			BeebView_SetBitmapPixels(hWnd);
			break;
		case IDM_MODE1:
			BeebView_SetMode(1);
			BeebView_SetBitmapPixels(hWnd);
			break;
		case IDM_MODE2:
			BeebView_SetMode(2);
			BeebView_SetBitmapPixels(hWnd);
			break;
		case IDM_MODE4:
			BeebView_SetMode(4);
			BeebView_SetBitmapPixels(hWnd);
			break;
		case IDM_MODE5:
			BeebView_SetMode(5);
			BeebView_SetBitmapPixels(hWnd);
			break;
		case IDM_COL0:
			BeebView_CycleColour(0);
			BeebView_SetBitmapPixels(hWnd);
			break;
		case IDM_COL1:
			BeebView_CycleColour(1);
			BeebView_SetBitmapPixels(hWnd);
			break;
		case IDM_COL2:
			BeebView_CycleColour(2);
			BeebView_SetBitmapPixels(hWnd);
			break;
		case IDM_COL3:
			BeebView_CycleColour(3);
			BeebView_SetBitmapPixels(hWnd);
			break;
		case IDM_COL4:
			BeebView_CycleColour(4);
			BeebView_SetBitmapPixels(hWnd);
			break;
		case IDM_COL5:
			BeebView_CycleColour(5);
			BeebView_SetBitmapPixels(hWnd);
			break;
		case IDM_COL6:
			BeebView_CycleColour(6);
			BeebView_SetBitmapPixels(hWnd);
			break;
		case IDM_COL7:
			BeebView_CycleColour(7);
			BeebView_SetBitmapPixels(hWnd);
			break;
		case IDM_HELP:
			ShellExecute(NULL, "open", "http://www.nerdoftheherd.com/tools/beebview/help/", NULL, NULL, SW_SHOWNORMAL);
			break;
		case IDM_ABOUT:
			DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
			break;
	}
}

void BeebView_OnPaint(HWND hWnd)
{
	PAINTSTRUCT PaintStruct;
	int nWidth = BeebView_Width(hWnd, nMode);

	// begin paint & get the handle of the screen DC
	HDC ScreenDC = BeginPaint(hWnd, &PaintStruct);

	// create a DC for the bitmap (it already exists)
	HDC BitmapDC = CreateCompatibleDC(ScreenDC);

	// save handle of current bitmap & select bitmap
	HBITMAP OldBitmap = SelectBitmap(BitmapDC, TheBitmap);

	// paint the bitmap
	StretchBlt(ScreenDC, 0, 0, BV_WIDTH, iClientHeight, BitmapDC, 0, 0, nWidth, iBBCHeight, SRCCOPY);

	// select previous bitmap
	SelectBitmap(BitmapDC, OldBitmap);
	// release DC
	DeleteDC(BitmapDC);

	EndPaint(hWnd, &PaintStruct);
}

void BeebView_OnDestroy(HWND hWnd)
{
	// delete bitmap from memory
	DeleteBitmap(TheBitmap);

	PostQuitMessage(0);
}

// BeebView Code -----------------------------------------------------------------------------------

/* Update Window title with filename */

void BeebView_UpdateTitle(HWND hWnd)
{
   char szMode[2] = "";
   lstrcpy(szWindowTitle, szAppName);
   lstrcat(szWindowTitle, " - ");
   lstrcat(szWindowTitle, szFileTitle);
   lstrcat(szWindowTitle, "  [MODE");
   _itoa_s(nMode, szMode, 10);
   lstrcat(szWindowTitle, szMode);
   lstrcat(szWindowTitle, "]");
   SetWindowText(hWnd, szWindowTitle);
}

/* Initialise the palette */

void BeebView_InitPalette(void)
{
   int i;

// colour values 0 to 7 as (index * 255)
   for (i = 0; i < 8; i++) {
      pal_bit[i] = i;
      palette[i] = BeebView_GetColour(pal_bit[i]);
   }
// 'flashing' colours to grey
   for (i = 8; i < 16; i++) {
      palette[i] = RGB(128, 128, 128);
   }
}

/* Generate a colour value */

COLORREF BeebView_GetColour(int colour)
{
	switch(colour) {
		case 0:
			return RGB(0, 0, 0);
			break;
		case 1:
			return RGB(255, 0, 0);
			break;
		case 2:
			return RGB(0, 255, 0);
			break;
		case 3:
			return RGB(255, 255, 0);
			break;
		case 4:
			return RGB(0, 0, 255);
			break;
		case 5:
			return RGB(255, 0, 255);
			break;
		case 6:
			return RGB(0, 255, 255);
			break;
		case 7:
			return RGB(255, 255, 255);
			break;
		default:
			return RGB(0, 0, 0);
	}
}

// Change mode, and set the correct default colours for the mode.

void BeebView_SetMode(int mode) {
	nMode = mode;

	switch(mode) {
		case 0:
		case 4:
			pal_bit[0] = 0;
			pal_bit[7] = 7;
			break;
		case 1:
		case 5:
			pal_bit[0] = 0;
			pal_bit[1] = 1;
			pal_bit[2] = 3;
			pal_bit[3] = 7;
			break;
		case 2:
			for (int i = 0; i < 8; i++) {
				pal_bit[i] = i;
			}
			break;
	}

	for (int i = 0; i < 8; i++) {
		palette[i] = BeebView_GetColour(pal_bit[i]);
	}
}

/* Cycle the colour palette */

void BeebView_CycleColour(int colour)
{
   pal_bit[colour]++;
   if(pal_bit[colour] > 7){
      pal_bit[colour] = 0;
   }
   palette[colour] = BeebView_GetColour(pal_bit[colour]);
}

/* Open the file, create the bitmap and display it */

void BeebView_SetBitmapPixels(HWND hWnd)
{
	HANDLE hFileHandle;
	int nWidth = BeebView_Width(hWnd, nMode);

	// check for empty filename string
	if(strlen(szFileName) == 0) {
		BeebView_UpdateTitle(hWnd);
		return;
	}

	// open the file
	hFileHandle = CreateFile(szFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
	if(hFileHandle == INVALID_HANDLE_VALUE) {
		char fileMessage[50+MAX_PATH];
		strcpy_s(fileMessage, "There was a problen opening the file '");
		strcat_s(fileMessage, szFileName);
		strcat_s(fileMessage, "'.");
		MessageBox(hWnd, fileMessage, "File Error", MB_ICONEXCLAMATION | MB_OK);
		return;
	}

	// Work out how many rows of blocks there are in this file.
	int iFileSize = GetFileSize(hFileHandle, NULL);
	int iBlocksInFile = iFileSize / BYTES;
	int iBlockRows;

	switch(nMode)
	{
		case 0:
			iBlockRows = iBlocksInFile / BBC_XBLKS0;
			break;
		case 1:
			iBlockRows = iBlocksInFile / BBC_XBLKS1;
			break;
		case 2:
			iBlockRows = iBlocksInFile / BBC_XBLKS2;
			break;
		case 4:
			iBlockRows = iBlocksInFile / BBC_XBLKS4;
			break;
		case 5:
			iBlockRows = iBlocksInFile / BBC_XBLKS5;
			break;
		default:
			MessageBox(hWnd, "Invalid screen mode!", "Program Error", MB_ICONASTERISK | MB_OK);
	}

	if(iBlockRows == 0) {
		// The file doesn't have even one row of blocks
		char tooSmallMessage[50+MAX_PATH];
		strcpy_s(tooSmallMessage, "The file '");
		strcat_s(tooSmallMessage, szFileName);
		strcat_s(tooSmallMessage, "' is too small to be a BBC graphics file.");
		MessageBox(hWnd, tooSmallMessage, "File Error", MB_ICONEXCLAMATION | MB_OK);

		// Close the file & abandon loading.
		CloseHandle(hFileHandle);
		return;
	}

	// Calculate how tall the image is based on the number of rows of blocks.
	iBBCHeight = iBlockRows * BYTES;
	iClientHeight = iBBCHeight * 2;

	// update the window title bar
	BeebView_UpdateTitle(hWnd);

	// delete the old bitmap
	DeleteBitmap(TheBitmap);

	// get the handle of the screen DC
	HDC ScreenDC = GetDC(hWnd);

	// create a bitmap, compatible with the screen
	TheBitmap = CreateCompatibleBitmap(ScreenDC, nWidth, iBBCHeight);

	// create a DC for it
	HDC BitmapDC = CreateCompatibleDC(ScreenDC);

	// release the screen DC
	ReleaseDC(hWnd, ScreenDC);

	// save handle of old bitmap & select new bitmap
	HBITMAP OldBitmap = SelectBitmap(BitmapDC, TheBitmap);

	// set pixels
	switch(nMode)
	{
		case 0:
		case 4:
			BeebView_MakePic04(hWnd, hFileHandle, BitmapDC, iBlockRows);
			break;
		case 1:
		case 5:
			BeebView_MakePic15(hWnd, hFileHandle, BitmapDC, iBlockRows);
			break;
		case 2:
			BeebView_MakePic2(hWnd, hFileHandle, BitmapDC, iBlockRows);
			break;
	}
	// close the file
	CloseHandle(hFileHandle);

	// select previous bitmap
	SelectBitmap(BitmapDC, OldBitmap);

	// release bitmap DC
	DeleteDC(BitmapDC);

	// Resize the window
	SetWindowPos (hWnd, NULL, 0, 0, WindowWidth(), WindowHeight(iClientHeight), SWP_NOMOVE | SWP_NOZORDER);

	// invalidate the client area to force a repaint
	InvalidateRect(hWnd, NULL, TRUE);
}

/* calculate the BBC screen width appropriate to the screen mode */

int BeebView_Width(HWND hWnd, int mode)
{
   switch(mode)
   {
   case 0:
   	return(BBC_WIDTH0);

   case 1:
   	return(BBC_WIDTH1);

   case 2:
   	return(BBC_WIDTH2);

   case 4:
   	return(BBC_WIDTH4);

   case 5:
   	return(BBC_WIDTH5);

   default:
      MessageBox(hWnd, "Invalid screen mode!", "Program Error", MB_ICONASTERISK | MB_OK);
		return(0);
   }
}

/* MODE 0 or MODE 4 picture */

void BeebView_MakePic04(HWND hWnd, HANDLE hFileHandle, HDC BitmapDC, int iYBlocks)
{
   int bit, i, j, k;
   unsigned int Byte;
   unsigned int index;
   COLORREF colour[2];
   DWORD bytesRead;
   int nX = 0;
   int nY = 0;
   char buffer[8];
   int nBlocks = BBC_XBLKS0;
   
   if(nMode == 4) {
      nBlocks = BBC_XBLKS4;
   }

   colour[0] = palette[0];
   colour[1] = palette[7];

   for(k = 0; k < iYBlocks; k++) {
      for(j = 0; j < nBlocks; j++) {
		 ReadFile(hFileHandle, &buffer, BYTES, &bytesRead, NULL);
         if(bytesRead < BYTES) {
            MessageBox(hWnd, "Out of data", "File Error", MB_ICONEXCLAMATION | MB_OK);
			return;
		 }
         for(i = 0; i < BYTES; i++) {
            Byte = buffer[i];
            for(bit = 0; bit < 8; bit++) {
                index = Byte & 1;
               SetPixel(BitmapDC, (nX+7)-bit, nY+i, colour[index]);
               Byte = Byte >> 1;
            }
         }
         nX = nX + 8;
      }
   nX = 0;
   nY = nY + 8;
   }
}

/* MODE 1 or MODE 5 picture */

void BeebView_MakePic15(HWND hWnd, HANDLE hFileHandle, HDC BitmapDC, int iYBlocks)
{
   int i, j, k;
   unsigned int Byte;
   unsigned int index;
   DWORD bytesRead;
   int nX = 0;
   int nY = 0;
   char buffer[8];
   int nBlocks = BBC_XBLKS1;

   if(nMode > 3) {
      nBlocks = BBC_XBLKS5;
   }

   for(k = 0; k < iYBlocks; k++) {
      for(j = 0; j < nBlocks; j++) {
	     ReadFile(hFileHandle, &buffer, BYTES, &bytesRead, NULL);
         if(bytesRead < BYTES) {
            MessageBox(hWnd, "Out of data", "File Error", MB_ICONEXCLAMATION | MB_OK);
            return;
         }
         for(i = 0; i < BYTES; i++) {
				Byte = buffer[i];
				index = ((Byte >> 6) & 2) | ((Byte >> 3) & 1);
				SetPixel(BitmapDC, nX, nY+i, palette[index]);
				index = ((Byte >> 5) & 2) | ((Byte >> 2) & 1);
				SetPixel(BitmapDC, nX+1, nY+i, palette[index]);
				index = ((Byte >> 4) & 2) | ((Byte >> 1) & 1);
				SetPixel(BitmapDC, nX+2, nY+i, palette[index]);
				index = ((Byte >> 3) & 2) | (Byte  & 1);
				SetPixel(BitmapDC, nX+3, nY+i, palette[index]);
			}
			nX = nX + 4;
		}
		nX = 0;
      nY = nY + 8;
	}
}

/* MODE 2 picture */

void BeebView_MakePic2(HWND hWnd, HANDLE hFileHandle, HDC BitmapDC, int iYBlocks)
{
   int i, j, k;
   unsigned int Byte;
   unsigned int index;
   DWORD bytesRead;
   int nX = 0;
   int nY = 0;
   char buffer[8];
   int nBlocks = BBC_XBLKS2;

// set colours
   for(k = 0; k < iYBlocks; k++) {
      for(j = 0; j < nBlocks; j++) {
	     ReadFile(hFileHandle, &buffer, BYTES, &bytesRead, NULL);
         if(bytesRead < BYTES) {
            MessageBox(hWnd, "Out of data", "File Error", MB_ICONEXCLAMATION | MB_OK);
            return;
         }
         for(i = 0; i < BYTES; i++) {
            Byte = buffer[i];
            index = ((Byte >> 4) & 8) | ((Byte >> 3) & 4) | ((Byte >> 2) & 2) | ((Byte >> 1) & 1);
            SetPixel(BitmapDC, nX, nY+i, palette[index]);
            index = ((Byte >> 3) & 8) | ((Byte >> 2) & 4) | ((Byte >> 1) & 2) | (Byte  & 1);
            SetPixel(BitmapDC, nX+1, nY+i, palette[index]);
         }
         nX = nX + 2;
      }
      nX = 0;
      nY = nY + 8;
   }
}

void BeebView_SaveBitmap(HWND hWnd) {
	int nWidth = BeebView_Width(hWnd, nMode);

	// get the handle of the screen DC
	HDC ScreenDC = GetDC(hWnd);

	// create a DC for the bitmap (it already exists)
	HDC BitmapDC = CreateCompatibleDC(ScreenDC);

	// save handle of current bitmap & select bitmap
	HBITMAP OldBitmap = SelectBitmap(BitmapDC, TheBitmap);

	// create a bitmap, compatible with the screen
	HBITMAP SizedBitmap = CreateCompatibleBitmap(ScreenDC, BV_WIDTH, iClientHeight);

	// create a DC for it
	HDC SizedDC = CreateCompatibleDC(ScreenDC);

	HBITMAP OldSizedBitmap = SelectBitmap(SizedDC, SizedBitmap);

	// paint the bitmap
	int res=StretchBlt(SizedDC, 0, 0, BV_WIDTH, iClientHeight, BitmapDC, 0, 0, nWidth, iBBCHeight, SRCCOPY);

	// Save the bitmap
	SaveDib(SizedDC, szSaveFileName, true);

	// select previous bitmap
	SelectBitmap(BitmapDC, OldBitmap);
	// release DC
	DeleteDC(BitmapDC);
	DeleteDC(SizedDC);

	// release the screen DC
	ReleaseDC(hWnd, ScreenDC);
}

// Utility Functions -------------------------------------------------------------------------------

BOOL OpenDialog(HWND hwndOwner, LPSTR filter, LPSTR fil, UINT iFilLen,
                LPSTR dlgtitle=NULL, LPSTR filtitle=NULL, UINT iFilTitleLen=0)
{
    OPENFILENAME opfil;
    memset((LPOPENFILENAME)&opfil,0,sizeof(opfil));
    opfil.lStructSize = sizeof(opfil);
	opfil.hwndOwner = hwndOwner;
    opfil.hInstance = NULL;	                    //application instance
    opfil.lpstrFilter = filter;                 //filter of files separated by \0
    opfil.lpstrFile = fil;                      //absolute path of filename
    opfil.nMaxFile = iFilLen;                   //length of filename buffer    
    opfil.lpstrFileTitle = filtitle;            //filename with no path
    opfil.nMaxFileTitle = iFilTitleLen;         //length of filename buffer
    opfil.lpstrTitle = dlgtitle;                //title of dialog box
    opfil.Flags = OFN_HIDEREADONLY;             //optional flags

	BOOL bResult = GetOpenFileName(&opfil);
	
	// Trim the extension off the file title if there is a '.' in the title.
	char *pPos = strrchr(filtitle, '.');
	if(pPos != NULL) {
		strncpy_s(filtitle, MAX_PATH, filtitle, (int)(pPos - filtitle));
	}

	return bResult;
}

BOOL SaveDialog(HWND hwndOwner, LPSTR filter, LPSTR fil, UINT iFilLen,
                LPSTR dlgtitle=NULL, LPSTR filtitle=NULL, UINT iFilTitleLen=0, LPSTR lpstrDefExt=NULL)
{
    OPENFILENAME opfil;
    memset((LPOPENFILENAME)&opfil,0,sizeof(opfil));
    opfil.lStructSize = sizeof(opfil);
	opfil.hwndOwner = hwndOwner;
    opfil.hInstance = NULL;	                    //application instance
    opfil.lpstrFilter = filter;                 //filter of files separated by \0
    opfil.lpstrFile = fil;                      //absolute path of filename
    opfil.nMaxFile = iFilLen;                   //length of filename buffer    
    opfil.lpstrFileTitle = filtitle;            //filename with no path
    opfil.nMaxFileTitle = iFilTitleLen;         //length of filename buffer
	opfil.lpstrDefExt = lpstrDefExt;			//Default file extension
    opfil.lpstrTitle = dlgtitle;                //title of dialog box
    opfil.Flags = OFN_HIDEREADONLY;             //optional flags    
    return GetSaveFileName(&opfil);
}

BOOL CenterWindow (HWND hwndChild, HWND hwndParent) 
{ 
        RECT    rChild, rParent; 
        int     wChild, hChild, wParent, hParent; 
        int     wScreen, hScreen, xNew, yNew; 
        HDC     hdc; 


        // Get the Height and Width of the child window 
        GetWindowRect (hwndChild, &rChild); 
        wChild = rChild.right - rChild.left; 
        hChild = rChild.bottom - rChild.top; 


        // Get the Height and Width of the parent window 
        GetWindowRect (hwndParent, &rParent); 
        wParent = rParent.right - rParent.left; 
        hParent = rParent.bottom - rParent.top; 


        // Get the display limits 
        hdc = GetDC (hwndChild); 
        wScreen = GetDeviceCaps (hdc, HORZRES); 
        hScreen = GetDeviceCaps (hdc, VERTRES); 
        ReleaseDC (hwndChild, hdc); 


        // Calculate new X position, then adjust for screen 
        xNew = rParent.left + ((wParent - wChild) /2); 
        if (xNew < 0) { 
                xNew = 0; 
        } else if ((xNew+wChild) > wScreen) { 
                xNew = wScreen - wChild; 
        } 


        // Calculate new Y position, then adjust for screen 
        yNew = rParent.top  + ((hParent - hChild) /2); 
        if (yNew < 0) { 
                yNew = 0; 
        } else if ((yNew+hChild) > hScreen) { 
                yNew = hScreen - hChild; 
        } 

		// Set it, and return 
        return SetWindowPos (hwndChild, NULL, xNew, yNew, 0, 0, SWP_NOSIZE | SWP_NOZORDER); 
}

int WindowHeight(int iClientHeight) {
	return iClientHeight + GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYMENU) + (GetSystemMetrics(SM_CYFIXEDFRAME)*2);
}

int WindowWidth() {
	return BV_WIDTH + (GetSystemMetrics(SM_CXFIXEDFRAME)*2);
}

void SaveDib(HDC hDC, LPCTSTR lpszFileName, BOOL bOverwriteExisting)
{
    HBITMAP hBitmap = (HBITMAP)GetCurrentObject(hDC, OBJ_BITMAP);

    // The .BMP file format is as follows:
    // BITMAPFILEHEADER / BITMAPINFOHEADER / color table / pixel data

    // We need the pixel data and the BITMAPINFOHEADER.
    // We can get both by using GetDIBits:
    BITMAP bitmapObject;
    GetObject(hBitmap, sizeof(BITMAP), &bitmapObject);

    BITMAPINFO *bmi = (BITMAPINFO *)_alloca(sizeof(BITMAPINFOHEADER)
                        + 256*sizeof(RGBQUAD));
    memset(&bmi->bmiHeader, 0, sizeof(BITMAPINFOHEADER));
    bmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

    int scanLineCount = GetDIBits(hDC, hBitmap, 0, bitmapObject.bmHeight,
                        NULL, bmi, DIB_RGB_COLORS);

    // This is important:
    // GetDIBits will, by default, set this to BI_BITFIELDS.
    bmi->bmiHeader.biCompression = BI_RGB;

    int imageBytes = dib::GetBitmapBytes(&bmi->bmiHeader);
    char *pBits = (char *)malloc(imageBytes);

    scanLineCount = GetDIBits(hDC, hBitmap, 0, bitmapObject.bmHeight,
                        pBits, bmi, DIB_RGB_COLORS);

    // OK, so we've got the bits, and the BITMAPINFOHEADER.
    // Now we can put them in a file.
    HANDLE hFile = CreateFile(lpszFileName, GENERIC_WRITE, 0, NULL,
                    bOverwriteExisting ? CREATE_ALWAYS : CREATE_NEW,
                    0, NULL);
    if (hFile != INVALID_HANDLE_VALUE)
    {
        // We don't deal with writing anything else at the moment.
        assert(bmi->bmiHeader.biBitCount == 32);

        // .BMP file begins with a BITMAPFILEHEADER,
        // so we'll write that.
        BITMAPFILEHEADER bmfh;
        bmfh.bfType = MAKEWORD('B','M');
        bmfh.bfSize = sizeof(BITMAPFILEHEADER)
                    + sizeof(BITMAPINFOHEADER)
                    + (bmi->bmiHeader.biClrUsed * sizeof(RGBQUAD))
                    + bmi->bmiHeader.biSizeImage;
        bmfh.bfReserved1 = 0;
        bmfh.bfReserved2 = 0;
        bmfh.bfOffBits = sizeof(BITMAPFILEHEADER)
                    + sizeof(BITMAPINFOHEADER)
                    + (bmi->bmiHeader.biClrUsed * sizeof(RGBQUAD));

        DWORD bytesWritten;
        WriteFile(hFile, &bmfh, sizeof(BITMAPFILEHEADER),
                    &bytesWritten, NULL);

        // Then it's followed by the BITMAPINFOHEADER structure
        WriteFile(hFile, &bmi->bmiHeader, sizeof(BITMAPINFOHEADER),
                    &bytesWritten, NULL);
        
        // Then the colour table.
        // ...which we don't support yet.

        // Then the pixel data.
        WriteFile(hFile, pBits, imageBytes, &bytesWritten, NULL);

        CloseHandle(hFile);
    }

    free(pBits);
}

namespace dib
{
    int GetBytesPerPixel(int depth)
    {
        return (depth==32 ? 4 : 3);
    }

    int GetBytesPerRow(int width, int depth)
    {
        int bytesPerPixel = GetBytesPerPixel(depth);
        int bytesPerRow = ((width * bytesPerPixel + 3) & ~3);

        return bytesPerRow;
    }

    int GetBitmapBytes(int width, int height, int depth)
    {
        return height * GetBytesPerRow(width, depth);
    }

    int GetBitmapBytes(const BITMAPINFOHEADER *bmih)
    {
        return GetBitmapBytes(bmih->biWidth, bmih->biHeight, bmih->biBitCount);
    }

    template <class Transform>
        void ApplyBitmapTransform(const BITMAPINFO *bmi,
                const void *pSourceBits, void *pDestBits, const Transform & trans)
    {
        ASSERT(bmi->bmiHeader.biBitCount == 32);

        const RGBQUAD *pSource = (const RGBQUAD *)pSourceBits;
        RGBQUAD *pDest = (RGBQUAD *)pDestBits;

        for (int y = 0; y < bmi->bmiHeader.biHeight; ++y)
        {
            for (int x = 0; x < bmi->bmiHeader.biWidth; ++x)
            {
                int offset = x + (bmi->bmiHeader.biWidth * y);

                RGBQUAD src = pSource[offset];
                RGBQUAD dst = trans(src);
                pDest[offset] = dst;
            }
        }
    }

    class ColourShiftTransform
    {
    protected:
        // Shift the channel down a little,
        // making sure that we don't wrap.
        BYTE Downshift(BYTE b, BYTE down_by) const
        {
            if (b > down_by)
                return b - down_by;

            return 0;
        }

        // Shift the channel up a little,
        // making sure that we saturate, rather than wrap.
        BYTE Upshift(BYTE b, BYTE up_by) const
        {
            if (b < (255 - up_by))
                return b + up_by;

            return 255;
        }
    };

    class RedShiftTransform : public ColourShiftTransform
    {
    public:
        RGBQUAD operator() (RGBQUAD src) const
        {
            RGBQUAD result;

            result.rgbBlue = Downshift(src.rgbBlue, 40);
            result.rgbGreen = Downshift(src.rgbGreen, 40);
            result.rgbRed = Upshift(src.rgbRed, 40);
            result.rgbReserved = src.rgbReserved;

            return result;
        }
    };
};
