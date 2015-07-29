#include "ViewController.h"
#include "TextView.h"
#include "UnicodeFile.h"
#include "WordWrapLayout.h"
#include "WordItemDelegate.h"
#include "TextViewStyle.h"
#include <assert.h>

namespace kofax
{

ViewController::ViewController(HWND hwndParent, const wchar_t* fileName)
	: m_view(new TextView(hwndParent))
	, m_style(new TextViewStyle(L"Arial", 24, RGB(0, 0, 0), RGB(128, 128, 128)))
{
	m_view->SetStyle(m_style);

	std::locale locale;
	m_model = UnicodeFile::OpenUnicodeFile(fileName, locale);
	assert(m_model);

	std::unique_ptr<WordItemDelegate> itemDelegate(new WordItemDelegate(m_model, locale));
	itemDelegate->SetStyles(new TextViewStyle(L"Arial", 18, RGB(0, 0, 0), RGB(128, 128, 128)),
		new TextViewStyle(L"Arial", 24, RGB(0, 0, 0), RGB(128, 128, 128)));

	RECT rect;
	m_view->GetClientRect(rect);
	m_view->SetLayout(new WordWrapLayout(*m_view, rect, itemDelegate.release()));
	m_view->UpdateView();
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