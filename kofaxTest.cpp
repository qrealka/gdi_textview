// kofaxTest.cpp : Defines the entry point for the application.
//

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <tchar.h>
#include <shellapi.h>
#include <Objbase.h>
#include "kofaxTest.h"
#include "ScopeGuard.h"

namespace
{
	const int MAX_LOADSTRING = 100;
	HINSTANCE hInst;								// current instance
	TCHAR     szTitle[MAX_LOADSTRING];				// The title bar text
	TCHAR     szWindowClass[MAX_LOADSTRING];		// the main window class name
	HWND      hwndTextView;
	TCHAR	  **argv;
	int		  argc;

	TCHAR **GetArgvCommandLine(int *argc)
	{
#ifdef UNICODE
		return CommandLineToArgvW(GetCommandLineW(), argc);
#else
		static_assert(false, "Code don't support MBCS mode. Only unicode configuration!");
		*argc = __argc;
		return __argv;
#endif
	}
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
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
	int wmId;

	switch (message)
	{
	case WM_CREATE:
		hwndTextView = CreateTextView(hWnd);
		return 0;

	case WM_SETFOCUS:
		SetFocus(hwndTextView);
		return 0;

	case WM_SIZE:
		MoveWindow(hwndTextView, 0, 0, static_cast<short>(LOWORD(lParam)), static_cast<short>(HIWORD(lParam)), TRUE);
		return 0;

	case WM_COMMAND:
		wmId = LOWORD(wParam);
		switch (wmId)
		{
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;

	case WM_CLOSE:
		DestroyWindow(hWnd);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
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

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_KOFAXTEST));
	wcex.hCursor		= LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground	= reinterpret_cast<HBRUSH>(COLOR_WINDOW+1);
	wcex.lpszMenuName   = nullptr;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
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
   hInst = hInstance; // Store instance handle in our global variable

   auto hWnd = CreateWindow(
	   szWindowClass, 
	   szTitle, 
	   WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
       CW_USEDEFAULT, 
	   CW_USEDEFAULT,
	   500, 
	   320, 
	   NULL, 
	   NULL, 
	   hInstance, 
	   NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	argv = GetArgvCommandLine(&argc);
	if (argc != 2)
	{
		::MessageBox(nullptr, _T("First parameter of application must be specified. \nFor example: 'kofaxTest.exe c:\\test.txt'"), _T("Information"), MB_ICONINFORMATION | MB_OK);
		return FALSE;
	}

	if (FAILED(::CoInitialize(nullptr)))
	{
		::MessageBox(nullptr, _T("Cannot initialize COM! Application will be closed."), _T("Error"), MB_ICONHAND | MB_OK);
		return FALSE;
	}

	kofax::ScopeGuard comUninit = [&]()
	{
		::CoUninitialize();
	};

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_KOFAXTEST, szWindowClass, MAX_LOADSTRING);
	
	MyRegisterClass(hInstance);
	RegisterTextView(hInstance, argv[1]);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	auto hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_KOFAXTEST));

	// Main message loop:
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return static_cast<int>(msg.wParam);
}
