#pragma once

#include "resource.h"

//#define MAXFILENAME 256 	    // maximum length of file pathname

#define BV_WIDTH  640           // width  of "screen" (client area)
#define BV_HEIGHT 512           // height of "screen" (client area)

#define BBC_WIDTH0 640          // width of BBC MODE0 screen
#define BBC_WIDTH1 320          //       "      MODE1   "   
#define BBC_WIDTH2 160          //       "      MODE2   "   
#define BBC_WIDTH4 320          //       "      MODE4   "   
#define BBC_WIDTH5 160          //       "      MODE5   "    
#define BBC_HEIGHT 256          // height of BBC screen 
#define BYTES   8               // number bytes in a block 
#define YBLOCKS 32              // vertical number of 8 byte blocks 

// Variables
HBITMAP TheBitmap;
// file type filters
char szFilterSpec [128] = "BBC Graphics Files (*.BBG)\0*.BBG\0All Files (*.*)\0*.*\0";
char szFileName[MAX_PATH] = "";
char szFileTitle[MAX_PATH] = "";
char szWindowTitle[80];

// Message Handler Functions
BOOL BeebView_OnCreate(HWND hWnd, CREATESTRUCT FAR* lpCreateStruct);
void BeebView_OnCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify);
void BeebView_OnPaint(HWND hWnd);
void BeebView_OnDestroy(HWND hWnd);

// BeebView functions
void BeebView_UpdateTitle(HWND);
void BeebView_InitPalette(void);
COLORREF BeebView_GetColour(int);
void BeebView_CycleColour(int);
void BeebView_SetBitmapPixels(HWND);
int  BeebView_Width(HWND, int);
void BeebView_MakePic04(HWND, HFILE, HDC);
void BeebView_MakePic15(HWND, HFILE, HDC);
void BeebView_MakePic2(HWND, HFILE, HDC);
void BeebView_SaveBitmap(HWND hWnd);

// Util Functions
BOOL OpenDialog(HWND hwndOwner, LPSTR filter, LPSTR fil, UINT iFilLen, LPSTR dlgtitle, LPSTR filtitle, UINT iFilTitleLen);
BOOL CenterWindow (HWND hwndChild, HWND hwndParent);


void SaveDib(HDC hDC, LPCTSTR lpszFileName, BOOL bOverwriteExisting);


namespace dib
{
    int GetBytesPerPixel(int depth);
    int GetBytesPerRow(int width, int depth);
    int GetBitmapBytes(int width, int height, int depth);
    int GetBitmapBytes(const BITMAPINFOHEADER *bmih);
};