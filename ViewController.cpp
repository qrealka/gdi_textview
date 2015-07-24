#include "ViewController.h"
#include "TextView.h"
#include "UnicodeFile.h"

namespace kofax
{

ViewController::ViewController(HWND hwndParent, const wchar_t* fileName)
	: m_view(new TextView(hwndParent))
{
	m_view->SetModel(UnicodeFile::OpenUnicodeFile(fileName));
}

LRESULT ViewController::WndProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	return m_view->WndProc(msg, wParam, lParam);
}

const TCHAR* ViewController::GetTypeName()
{
	return L"KofaxMainView";
}

}