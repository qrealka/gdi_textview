#ifndef _92476F8A_1918_49BA_8509_33A17BC80D56_
#define _92476F8A_1918_49BA_8509_33A17BC80D56_

#define _WIN32_WINNT 0x400
#include <windows.h>
#include <memory>

#include "IListView.h"

namespace kofax
{

struct IListModel;
struct IStyleView;

class ViewController
{
public:
	ViewController(HWND hwndParent, const wchar_t* fileName);
	LRESULT WndProc(UINT msg, WPARAM wParam, LPARAM lParam);

	static const TCHAR* GetTypeName();

private:
	std::unique_ptr<IListView> m_view;
	std::shared_ptr<IListModel> m_model;
	std::shared_ptr<IStyleView> m_style;
};

}

#endif
