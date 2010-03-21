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

#include "stdafx.h"
#include "Beebview.h"
#include "BbcScreen.h"
#include "Bitmap.h"
#include "About.h"
#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szAppName[MAX_LOADSTRING];			    // The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

BbcScreen *screen = NULL;

char currentFileTitle[MAX_PATH] = "";
bool bAutoSave = false;

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
 	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szAppName, MAX_LOADSTRING);
	LoadString(hInstance, IDC_BEEBVIEW, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Work through the command line parameters.
	// This block of code will set szFileName to the last quoted part of the command line, and pick up any other params.
	/* char *params;
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
	} */

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

// Message Handler Functions -----------------------------------------------------------------------

BOOL BeebView_OnCreate(HWND hWnd, CREATESTRUCT FAR* lpCreateStruct)
{
	//BeebView_SetBitmapPixels(hWnd);

	// Automatically save the file and exit if bAutoSave is true
	/* if(bAutoSave) {
		// Add .bmp to the loaded file.
		strcpy_s(szSaveFileName, szFileName);
		strcat_s(szSaveFileName, ".bmp");
		BeebView_SaveBitmap(hWnd);

		// Close the program
		DestroyWindow(hWnd);
	} */

	return TRUE;
}

void BeebView_OnInitMenuPopup(HWND hwnd, HMENU hMenu, UINT item, BOOL fSystemMenu)
{
	if(screen == NULL)
	{
		// Gray out the menu options as no image is loaded
		EnableMenuItem(hMenu, IDM_SAVEAS, MF_GRAYED);
		EnableMenuItem(hMenu, IDM_MODE0, MF_GRAYED);
		EnableMenuItem(hMenu, IDM_MODE1, MF_GRAYED);
		EnableMenuItem(hMenu, IDM_MODE2, MF_GRAYED);
		EnableMenuItem(hMenu, IDM_MODE4, MF_GRAYED);
		EnableMenuItem(hMenu, IDM_MODE5, MF_GRAYED);
		EnableMenuItem(hMenu, IDM_COL0, MF_GRAYED);
		EnableMenuItem(hMenu, IDM_COL1, MF_GRAYED);
		EnableMenuItem(hMenu, IDM_COL2, MF_GRAYED);
		EnableMenuItem(hMenu, IDM_COL3, MF_GRAYED);
		EnableMenuItem(hMenu, IDM_COL4, MF_GRAYED);
		EnableMenuItem(hMenu, IDM_COL5, MF_GRAYED);
		EnableMenuItem(hMenu, IDM_COL6, MF_GRAYED);
		EnableMenuItem(hMenu, IDM_COL7, MF_GRAYED);
	}
	else
	{
		// Enable the Save As and Mode selection
		EnableMenuItem(hMenu, IDM_SAVEAS, MF_ENABLED);
		EnableMenuItem(hMenu, IDM_MODE0, MF_ENABLED);
		EnableMenuItem(hMenu, IDM_MODE1, MF_ENABLED);
		EnableMenuItem(hMenu, IDM_MODE2, MF_ENABLED);
		EnableMenuItem(hMenu, IDM_MODE4, MF_ENABLED);
		EnableMenuItem(hMenu, IDM_MODE5, MF_ENABLED);

		// Set a radio check next to the menu item for the current mode.
		UINT checkItem;

		switch(screen->getMode())
		{
			case 0:
				checkItem = IDM_MODE0;
				break;
			case 1:
				checkItem = IDM_MODE1;
				break;
			case 2:
				checkItem = IDM_MODE2;
				break;
			case 4:
				checkItem = IDM_MODE4;
				break;
			case 5:
				checkItem = IDM_MODE5;
				break;
		} 
		
		CheckMenuRadioItem(hMenu, IDM_MODE0, IDM_MODE5, checkItem, MF_BYCOMMAND);

		// Enable the applicable colour cycle options for the current mode
		EnableMenuItem(hMenu, IDM_COL0, MF_ENABLED);

		switch(screen->getMode()) {
			case 0:
			case 4:
				EnableMenuItem(hMenu, IDM_COL1, MF_ENABLED);
				EnableMenuItem(hMenu, IDM_COL2, MF_GRAYED);
				EnableMenuItem(hMenu, IDM_COL3, MF_GRAYED);
				EnableMenuItem(hMenu, IDM_COL4, MF_GRAYED);
				EnableMenuItem(hMenu, IDM_COL5, MF_GRAYED);
				EnableMenuItem(hMenu, IDM_COL6, MF_GRAYED);
				EnableMenuItem(hMenu, IDM_COL7, MF_GRAYED);
				break;
			case 1:
			case 5:
				EnableMenuItem(hMenu, IDM_COL1, MF_ENABLED);
				EnableMenuItem(hMenu, IDM_COL2, MF_ENABLED);
				EnableMenuItem(hMenu, IDM_COL3, MF_ENABLED);
				EnableMenuItem(hMenu, IDM_COL4, MF_GRAYED);
				EnableMenuItem(hMenu, IDM_COL5, MF_GRAYED);
				EnableMenuItem(hMenu, IDM_COL6, MF_GRAYED);
				EnableMenuItem(hMenu, IDM_COL7, MF_GRAYED);
				break;
			case 2:
				EnableMenuItem(hMenu, IDM_COL1, MF_ENABLED);
				EnableMenuItem(hMenu, IDM_COL2, MF_ENABLED);
				EnableMenuItem(hMenu, IDM_COL3, MF_ENABLED);
				EnableMenuItem(hMenu, IDM_COL4, MF_ENABLED);
				EnableMenuItem(hMenu, IDM_COL5, MF_ENABLED);
				EnableMenuItem(hMenu, IDM_COL6, MF_ENABLED);
				EnableMenuItem(hMenu, IDM_COL7, MF_ENABLED);
				break;
		}
	}
}

void BeebView_OnCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify)
{
	// Parse the menu selections:
	switch (id)
	{
		case IDM_OPEN:
			BeebView_OpenFile(hWnd);
			break;
		case IDM_SAVEAS:
			BeebView_SaveBitmap(hWnd);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case IDM_MODE0:
			screen->setMode(0);
			BeebView_ForceRepaint(hWnd);
			break;
		case IDM_MODE1:
			screen->setMode(1);
			BeebView_ForceRepaint(hWnd);
			break;
		case IDM_MODE2:
			screen->setMode(2);
			BeebView_ForceRepaint(hWnd);
			break;
		case IDM_MODE4:
			screen->setMode(4);
			BeebView_ForceRepaint(hWnd);
			break;
		case IDM_MODE5:
			screen->setMode(5);
			BeebView_ForceRepaint(hWnd);
			break;
		case IDM_COL0:
			BeebView_CycleColour(0);
			BeebView_ForceRepaint(hWnd);
			break;
		case IDM_COL1:
			BeebView_CycleColour(1);
			BeebView_ForceRepaint(hWnd);
			break;
		case IDM_COL2:
			BeebView_CycleColour(2);
			BeebView_ForceRepaint(hWnd);
			break;
		case IDM_COL3:
			BeebView_CycleColour(3);
			BeebView_ForceRepaint(hWnd);
			break;
		case IDM_COL4:
			BeebView_CycleColour(4);
			BeebView_ForceRepaint(hWnd);
			break;
		case IDM_COL5:
			BeebView_CycleColour(5);
			BeebView_ForceRepaint(hWnd);
			break;
		case IDM_COL6:
			BeebView_CycleColour(6);
			BeebView_ForceRepaint(hWnd);
			break;
		case IDM_COL7:
			BeebView_CycleColour(7);
			BeebView_ForceRepaint(hWnd);
			break;
		case IDM_HELP:
			ShellExecute(NULL, "open", "http://www.nerdoftheherd.com/tools/beebview/help/", NULL, NULL, SW_SHOWNORMAL);
			break;
		case IDM_REPORTBUG:
			ShellExecute(NULL, "open", "http://www.nerdoftheherd.com/tools/beebview/bug_report.php", NULL, NULL, SW_SHOWNORMAL);
			break;
		case IDM_ABOUT:
			DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
			break;
	}
}

void BeebView_OnPaint(HWND hWnd)
{
	PAINTSTRUCT PaintStruct;

	// begin paint & get the handle of the screen DC
	HDC screenDC = BeginPaint(hWnd, &PaintStruct);

	if(screen != NULL)
	{
		// Create a DC for the bitmap & select bitmap
		HDC bitmapDC = CreateCompatibleDC(screenDC);
		HBITMAP oldBitmap = SelectBitmap(bitmapDC, screen->getBitmap());

		// paint the bitmap
		StretchBlt(screenDC, 0, 0, BV_WIDTH, dispHeight(screen->getScreenHeight()), bitmapDC, 0, 0, screen->getScreenWidth(), screen->getScreenHeight(), SRCCOPY);

		// select previous bitmap & release the DC
		SelectBitmap(bitmapDC, oldBitmap);
		DeleteDC(bitmapDC);
	}

	EndPaint(hWnd, &PaintStruct);
}

void BeebView_OnDestroy(HWND hWnd)
{
	// delete bitmap from memory
	//DeleteBitmap(TheBitmap);

	PostQuitMessage(0);
}

// BeebView Code -----------------------------------------------------------------------------------

// Cycle a colour in the palette
void BeebView_CycleColour(int colour)
{
	screen->setColour(colour, (screen->getColour(colour) + 1) % 8);
}

void BeebView_OpenFile(HWND hWnd)
{
	char fileName[MAX_PATH] = "";
	char fileTitle[MAX_PATH] = "";
	
	OPENFILENAME opfil;
	memset((LPOPENFILENAME)&opfil,0,sizeof(opfil));
	opfil.lStructSize = sizeof(opfil);
	opfil.hwndOwner = hWnd;
	opfil.hInstance = NULL;	              //application instance
	opfil.lpstrFilter = LOADFILTER;       //filter of files separated by \0
	opfil.lpstrFile = fileName;           //absolute path of filename
	opfil.nMaxFile = MAX_PATH;            //length of filename buffer    
	opfil.lpstrFileTitle = fileTitle;     //filename with no path
	opfil.nMaxFileTitle = MAX_PATH;       //length of filename buffer
	opfil.lpstrTitle = "Open file";       //title of dialog box
	opfil.Flags = OFN_HIDEREADONLY;       //optional flags

	int result = GetOpenFileName(&opfil);
	
	if(result != 0)
	{
		// Trim the extension off the file title if there is a '.' in it.
		char *dotPos = strrchr(fileTitle, '.');
	
		if(dotPos != NULL) {
			*dotPos = '\0';
		}
		
		strcpy_s(currentFileTitle, fileTitle);

		BeebView_LoadFile(hWnd, fileName);
	}
}

void BeebView_LoadFile(HWND hWnd, char *fileName)
{
	if(BeebView_LoadMemDump(hWnd, fileName))
	{
		BeebView_ForceRepaint(hWnd);
	}
}

void BeebView_ForceRepaint(HWND hWnd)
{
	// Update the window title bar
	int titleLen = strlen(szAppName) + 14 + strlen(currentFileTitle);
	char *windowTitle = new char[titleLen];
	sprintf_s(windowTitle, titleLen, "%s - %s  [MODE %d]", szAppName, currentFileTitle, screen->getMode());
	SetWindowText(hWnd, windowTitle);
	delete []windowTitle;

	screen->generateBitmap(hWnd);

	// Resize the window
	SetWindowPos(hWnd, NULL, 0, 0, WindowWidth(), WindowHeight(dispHeight(screen->getScreenHeight())), SWP_NOMOVE | SWP_NOZORDER);

	// Invalidate the client area to force a repaint
	InvalidateRect(hWnd, NULL, TRUE);
}

BOOL BeebView_LoadMemDump(HWND hWnd, char *fileName)
{
	HANDLE hFileHandle;

	// open the file
	hFileHandle = CreateFile(fileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);

	if(hFileHandle == INVALID_HANDLE_VALUE) {
		int messageLen = strlen(fileName) + 41;
		char *message = new char[messageLen];
		sprintf_s(message, messageLen, "There was a problem opening the file \"%s\".", fileName);
		MessageBox(hWnd, message, "File Error", MB_ICONEXCLAMATION | MB_OK);
		delete []message;
		return false;
	}

	// Initialise a new BbcScreen instance to store the data in
	int fileSize = GetFileSize(hFileHandle, NULL);
	screen = new BbcScreen(fileSize);

	DWORD bytesRead = 0;
	int writeAddr = 0;
	unsigned char buffer[READBUF];

	do
	{
		ReadFile(hFileHandle, &buffer, READBUF, &bytesRead, NULL);

		for(unsigned int xfer = 0; xfer < bytesRead; xfer++)
		{
			screen->setScreenBit(writeAddr, buffer[xfer]);
			writeAddr++;
		}
	} while(bytesRead > 0);

	// close the file
	CloseHandle(hFileHandle);

	return true;
}

void BeebView_SaveBitmap(HWND hWnd) {
	char saveFileName[MAX_PATH] = "";
	char saveFileTitle[MAX_PATH] = "";
	
	// Set the default file name
	strcpy_s(saveFileName, currentFileTitle);
	
	// Show the save as dialog
	OPENFILENAME opfil;
	memset((LPOPENFILENAME)&opfil,0,sizeof(opfil));
	opfil.lStructSize = sizeof(opfil);
	opfil.hwndOwner = hWnd;
	opfil.hInstance = NULL;	                 //application instance
	opfil.lpstrFilter = SAVEFILTER;          //filter of files separated by \0
	opfil.lpstrFile = saveFileName;          //absolute path of filename
	opfil.nMaxFile = MAX_PATH;               //length of filename buffer    
	opfil.lpstrFileTitle = saveFileTitle;    //filename with no path
	opfil.nMaxFileTitle = MAX_PATH;          //length of filename buffer
	opfil.lpstrDefExt = "bmp";               //Default file extension
	opfil.lpstrTitle = "Save As";            //title of dialog box
	opfil.Flags = OFN_HIDEREADONLY;          //optional flags    
	int result = GetSaveFileName(&opfil);

	if(result == 0)
	{
		return;
	}

	// Get the handle of the screen DC
	HDC screenDC = GetDC(hWnd);

	// Create a DC for the bitmap & select it
	HDC bitmapDC = CreateCompatibleDC(screenDC);
	HBITMAP oldBitmap = SelectBitmap(bitmapDC, screen->getBitmap());

	// Create a compatible bitmap for the resized image
	HBITMAP sizedBitmap = CreateCompatibleBitmap(screenDC, BV_WIDTH, dispHeight(screen->getScreenHeight()));
	
	// Create a DC for the resized image bitmap & select it
	HDC sizedDC = CreateCompatibleDC(screenDC);
	SelectBitmap(sizedDC, sizedBitmap);

	// Resize the bitmap to the output size
	StretchBlt(sizedDC, 0, 0, BV_WIDTH, dispHeight(screen->getScreenHeight()), bitmapDC, 0, 0, screen->getScreenWidth(), screen->getScreenHeight(), SRCCOPY);

	// Save the bitmap
	bmp::SaveBmp(sizedDC, saveFileName, true);

	// Select the original bitmap
	SelectBitmap(bitmapDC, oldBitmap);

	// Release the DCs
	DeleteDC(bitmapDC);
	DeleteDC(sizedDC);
	ReleaseDC(hWnd, screenDC);
}

int dispHeight(int bbcHeight)
{
	return bbcHeight * 2;
}

// Utility Functions -------------------------------------------------------------------------------

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
