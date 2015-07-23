#include "TextView.h"
#include "kofaxTest.h"
#include <tchar.h>

LRESULT WINAPI TextViewWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	using kofax::TextView;

	auto ptv = reinterpret_cast<TextView *>(GetWindowLongPtr(hwnd, 0));

	switch (msg)
	{
	case WM_NCCREATE:
	{
		try
		{
			if (ptv = new TextView(hwnd))
			{
				SetWindowLongPtr(hwnd, 0, reinterpret_cast<LONG>(ptv));
				return TRUE;
			}
			return FALSE;
		}
		catch (...)
		{
			return FALSE;
		}
	}

	case WM_NCDESTROY:
		delete ptv;
		SetWindowLongPtr(hwnd, 0, 0);
		return 0;

	default:
		{
			return ptv
				? ptv->WndProc(msg, wParam, lParam)
				: 0;
		}
	}
}

//
//	Register the TextView class
//
BOOL WINAPI RegisterTextView(HINSTANCE hInstance)
{
	WNDCLASSEX	wcx;

	wcx.cbSize = sizeof(wcx);
	wcx.style = 0;
	wcx.lpfnWndProc = TextViewWndProc;
	wcx.cbClsExtra = 0;
	wcx.cbWndExtra = sizeof(kofax::TextView *);
	wcx.hInstance = hInstance;
	wcx.hIcon = nullptr;
	wcx.hCursor = LoadCursor(nullptr, IDC_IBEAM);
	wcx.hbrBackground = static_cast<HBRUSH>(nullptr);		//stop flick 
	wcx.lpszMenuName = nullptr;
	wcx.lpszClassName = kofax::TextView::GetTextViewClassName();
	wcx.hIconSm = nullptr;

	return RegisterClassEx(&wcx) ? TRUE : FALSE;
}

//
//	Create a TextView window
//
HWND WINAPI CreateTextView(HWND hwndParent)
{
	return CreateWindowEx(WS_EX_CLIENTEDGE,
		kofax::TextView::GetTextViewClassName(), _T(""),
		WS_CHILD | WS_VISIBLE,
		0, 0, 0, 0,
		hwndParent,
		nullptr,
		GetModuleHandle(nullptr),
		nullptr);
}

