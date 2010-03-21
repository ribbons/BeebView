#pragma once

namespace bmp
{
	void SaveBmp(HDC hDC, LPCTSTR lpszFileName, BOOL bOverwriteExisting);
    int GetBytesPerPixel(int depth);
    int GetBytesPerRow(int width, int depth);
    int GetBitmapBytes(int width, int height, int depth);
    int GetBitmapBytes(const BITMAPINFOHEADER *bmih);
};
