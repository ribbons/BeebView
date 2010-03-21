#pragma once

#include "resource.h"

// Dialog message handler
LRESULT CALLBACK About(HWND, UINT, WPARAM, LPARAM);

// Specific message handlers
void About_InitDialog(HWND hDlg);
