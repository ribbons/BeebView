// Beebview.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Beebview.h"
#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szAppName[MAX_LOADSTRING];			    // The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

int nMode = 1;
int pal_bit[16];
COLORREF palette[16];

OFSTRUCT of;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
 	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szAppName, MAX_LOADSTRING);
	LoadString(hInstance, IDC_BEEBVIEW, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

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

   int nWidth = BV_WIDTH + (GetSystemMetrics(SM_CXFIXEDFRAME)*2);
   int nHeight = BV_HEIGHT + GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYMENU) + (GetSystemMetrics(SM_CYFIXEDFRAME)*2);

   hWnd = CreateWindow(szWindowClass, szAppName, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
      CW_USEDEFAULT, 0, nWidth, nHeight, NULL, NULL, hInstance, NULL);

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
		HANDLE_MSG(hWnd, WM_CREATE,  BeebView_OnCreate);
		HANDLE_MSG(hWnd, WM_COMMAND, BeebView_OnCommand);
		HANDLE_MSG(hWnd, WM_PAINT,   BeebView_OnPaint);
		HANDLE_MSG(hWnd, WM_DESTROY, BeebView_OnDestroy);
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
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return FALSE;
}

// Message Handler Functions -----------------------------------------------------------------------

BOOL BeebView_OnCreate(HWND hWnd, CREATESTRUCT FAR* lpCreateStruct)
{
	BeebView_InitPalette();
	BeebView_SetBitmapPixels(hWnd);
	return TRUE;
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
			SaveDialog(hWnd, szSaveFilterSpec, szSaveFileName, MAX_PATH, "Save file", szSaveFileTitle, MAX_PATH);
			BeebView_SaveBitmap(hWnd);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case IDM_MODE0:
			nMode = 0;
			BeebView_SetBitmapPixels(hWnd);
			break;
		case IDM_MODE1:
			nMode = 1;
			BeebView_SetBitmapPixels(hWnd);
			break;
		case IDM_MODE2:
			nMode = 2;
			BeebView_SetBitmapPixels(hWnd);
			break;
		case IDM_MODE4:
			nMode = 4;
			BeebView_SetBitmapPixels(hWnd);
			break;
		case IDM_MODE5:
			nMode = 5;
			BeebView_SetBitmapPixels(hWnd);
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
	StretchBlt(ScreenDC, 0, 0, BV_WIDTH, BV_HEIGHT, BitmapDC, 0, 0, nWidth, BBC_HEIGHT, SRCCOPY);

	/*if(bSave) {
		HDC StretchedDC = CreateCompatibleDC(ScreenDC);
		StretchBlt(StretchedDC, 0, 0, BV_WIDTH, BV_HEIGHT, BitmapDC, 0, 0, nWidth, BBC_HEIGHT, SRCCOPY);
		SaveDib(StretchedDC, szSaveName, true);
	}*/

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
   return(RGB((pal_bit[colour] & 1) * 255, ((pal_bit[colour] / 2) & 1) * 255, ((pal_bit[colour] / 4) & 1) * 255));
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
	HFILE hFileHandle;
	int nWidth = BeebView_Width(hWnd, nMode);

	// check for empty filename string
	if(strlen(szFileTitle) == 0) {
		BeebView_UpdateTitle(hWnd);
		return;
	}

	// open the file
	hFileHandle = OpenFile(szFileTitle, &of, OF_READ);
	if(hFileHandle == NULL) {
		MessageBox(hWnd, "Failed to open file", "File Error", MB_ICONEXCLAMATION | MB_OK);
		return;
	}

	// update the window title bar
	BeebView_UpdateTitle(hWnd);

	// delete the old bitmap
	DeleteBitmap(TheBitmap);

	// get the handle of the screen DC
	HDC ScreenDC = GetDC(hWnd);

	// create a bitmap, compatible with the screen
	TheBitmap = CreateCompatibleBitmap(ScreenDC, nWidth, BBC_HEIGHT);

	// create a DC for it
	HDC BitmapDC = CreateCompatibleDC(ScreenDC);

	// release the screen DC
	ReleaseDC(hWnd, ScreenDC);

	// save handle of old bitmap & select new bitmap
	HBITMAP OldBitmap = SelectBitmap(BitmapDC, TheBitmap);

	// set pixels
	if(hFileHandle != NULL) {
		switch(nMode)
		{
			case 0:
			case 4:
				BeebView_MakePic04(hWnd, hFileHandle, BitmapDC);
				break;
			case 1:
			case 5:
				BeebView_MakePic15(hWnd, hFileHandle, BitmapDC);
				break;
			case 2:
				BeebView_MakePic2(hWnd, hFileHandle, BitmapDC);
				break;
			default:
				MessageBox(hWnd, "Invalid screen mode!", "Program Error", MB_ICONASTERISK | MB_OK);
		}
		// close the file
		_lclose(hFileHandle);
	}

	// select previous bitmap
	SelectBitmap(BitmapDC, OldBitmap);

	// release bitmap DC
	DeleteDC(BitmapDC);

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

void BeebView_MakePic04(HWND hWnd, HFILE hFileHandle, HDC BitmapDC)
{
   int bit, i, j, k;
   unsigned int Byte;
   unsigned int index;
   COLORREF colour[2];
   int nBlocks = 20480 / BBC_HEIGHT;
   if(nMode > 3) {
      nBlocks = 10240 / BBC_HEIGHT;
   }
   int nX = 0;
   int nY = 0;
   char buffer[8];

   colour[0] = palette[0];
   colour[1] = palette[7];

   for(k = 0; k < YBLOCKS; k++) {
      for(j = 0; j < nBlocks; j++) {
         if(_lread(hFileHandle, buffer, BYTES) < BYTES) {
            MessageBox(hWnd, "Out of data", "File Error", MB_ICONEXCLAMATION | MB_OK);
         break;
				}
         for(i = 0; i < BYTES; i++) {
            Byte = buffer[i];
            for(bit = 0; bit < 8; bit++) {
                index = Byte & 1;
               SetPixel(BitmapDC, nX+8-bit, nY+i, colour[index]);
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

void BeebView_MakePic15(HWND hWnd, HFILE hFileHandle, HDC BitmapDC)
{
   int i, j, k;
   unsigned int Byte;
   unsigned int index;
   int nBlocks = 20480 / BBC_HEIGHT;
   if(nMode > 3) {
      nBlocks = 10240 / BBC_HEIGHT;
   }
   int nX = 0;
   int nY = 0;
   char buffer[8];

   for(k = 0; k < YBLOCKS; k++) {
      for(j = 0; j < nBlocks; j++) {
         if(_lread(hFileHandle, buffer, BYTES) < BYTES) {
            MessageBox(hWnd, "Out of data", "File Error", MB_ICONEXCLAMATION | MB_OK);
            break;
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

void BeebView_MakePic2(HWND hWnd, HFILE hFileHandle, HDC BitmapDC)
{
   int i, j, k;
   unsigned int Byte;
   unsigned int index;
   int nBlocks = 20480 / BBC_HEIGHT;
   int nX = 0;
   int nY = 0;
   char buffer[8];

// set colours
   for(k = 0; k < YBLOCKS; k++) {
      for(j = 0; j < nBlocks; j++) {
         if(_lread(hFileHandle, buffer, BYTES) < BYTES) {
            MessageBox(hWnd, "Out of data", "File Error", MB_ICONEXCLAMATION | MB_OK);
            break;
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
	HBITMAP SizedBitmap = CreateCompatibleBitmap(ScreenDC, BV_WIDTH, BV_HEIGHT);

	// create a DC for it
	HDC SizedDC = CreateCompatibleDC(ScreenDC);

	HBITMAP OldSizedBitmap = SelectBitmap(SizedDC, SizedBitmap);

	// paint the bitmap
	int res=StretchBlt(SizedDC, 0, 0, BV_WIDTH, BV_HEIGHT, BitmapDC, 0, 0, nWidth, BBC_HEIGHT, SRCCOPY);

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
    return GetOpenFileName(&opfil);    
}

BOOL SaveDialog(HWND hwndOwner, LPSTR filter, LPSTR fil, UINT iFilLen,
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