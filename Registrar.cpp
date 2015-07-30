#include "ViewController.h"
#include "kofaxTest.h"
#include <tchar.h>
#include <assert.h>

namespace
{

const wchar_t* controllerFileName;

LRESULT WINAPI TextViewWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	using kofax::ViewController;

	auto ptv = reinterpret_cast<ViewController *>(GetWindowLongPtr(hwnd, 0));

	switch (msg)
	{
	case WM_NCCREATE:
	{
		try
		{
			ptv = new ViewController(hwnd, controllerFileName);
			SetWindowLongPtr(hwnd, 0, reinterpret_cast<LONG>(ptv));
			return TRUE;
		}
		catch (const std::exception&)
		{
			assert(false);
			::MessageBox(hwnd, _T("Cannot create application controller!"), _T("Error"), MB_ICONHAND | MB_OK);
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

}

//
//	Register the TextView class
//
BOOL WINAPI RegisterTextView(HINSTANCE hInstance, const wchar_t* fileName)
{
	assert(fileName && fileName[0]);

	controllerFileName = fileName;

	WNDCLASSEX	wcx;

	wcx.cbSize = sizeof(wcx);
	wcx.style = 0;
	wcx.lpfnWndProc = TextViewWndProc;
	wcx.cbClsExtra = 0;
	wcx.cbWndExtra = sizeof(kofax::ViewController *);
	wcx.hInstance = hInstance;
	wcx.hIcon = nullptr;
	wcx.hCursor = LoadCursor(nullptr, IDC_IBEAM);
	wcx.hbrBackground = static_cast<HBRUSH>(nullptr);		//stop flick 
	wcx.lpszMenuName = nullptr;
	wcx.lpszClassName = kofax::ViewController::GetTypeName();
	wcx.hIconSm = nullptr;

	return RegisterClassEx(&wcx) ? TRUE : FALSE;
}

//
//	Create a TextView window
//
HWND WINAPI CreateTextView(HWND hwndParent)
{
	return CreateWindowEx(WS_EX_CLIENTEDGE,
		kofax::ViewController::GetTypeName(), _T(""),
		WS_CHILD | WS_VISIBLE,
		0, 0, 0, 0,
		hwndParent,
		nullptr,
		GetModuleHandle(nullptr),
		nullptr);
}

