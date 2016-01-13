/*
 * This file is part of BBC Graphics Viewer.
 * Copyright © 2007-2010 by the authors - see the AUTHORS file for details.
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

#include "stdafx.h"
#include "Bitmap.h"

namespace bmp
{
    void SaveBmp(HDC hDC, LPCTSTR lpszFileName, BOOL bOverwriteExisting)
    {
        HBITMAP hBitmap = (HBITMAP)GetCurrentObject(hDC, OBJ_BITMAP);

        // The .BMP file format is as follows:
        // BITMAPFILEHEADER / BITMAPINFOHEADER / optional color table / pixel data

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

        int imageBytes = GetBitmapBytes(&bmi->bmiHeader);
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

            // Then the pixel data.
            WriteFile(hFile, pBits, imageBytes, &bytesWritten, NULL);

            CloseHandle(hFile);
        }

        free(pBits);
    }

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
};
