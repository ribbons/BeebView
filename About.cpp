#include "stdafx.h"
#include "About.h"
#include "Beebview.h"

// Message handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
			About_InitDialog(hDlg);
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

void About_InitDialog(HWND hDlg)
{
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
}