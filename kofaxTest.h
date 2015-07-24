#pragma once

#include "resource.h"

#ifdef __cplusplus
extern "C" {
#endif

BOOL WINAPI	RegisterTextView(HINSTANCE hInstance, const wchar_t* fileName);
HWND WINAPI CreateTextView(HWND hwndParent);

#ifdef __cplusplus
}
#endif

