#ifndef _BFB60DF2_ABB4_47F6_95CD_E5EAA5FC02BE_
#define _BFB60DF2_ABB4_47F6_95CD_E5EAA5FC02BE_

#define _WIN32_WINNT 0x400
#include <windows.h>
#include <memory>

namespace kofax
{

struct IStringListModel;
struct IStringIndex;

class TextView 
{
public:
	explicit TextView(HWND hwndParent);

	void SetModel(IStringListModel* const model);
	//void SetTextLayout(IViewLayout* layout);
	//void SetItemDelegate(IItemStyleDelegate* itemDelegate);

	LRESULT WndProc(UINT msg, WPARAM wParam, LPARAM lParam);

	static const TCHAR* GetTextViewClassName();

private:
	TextView(const TextView&) = delete;
	TextView& operator=(const TextView&) = delete;

	void PaintBackground(HDC hdc, int x, int y, int width, long height)const;
	void PaintText(HDC hdc, const std::unique_ptr<const IStringIndex>& line, int x, int y, HRGN region)const;

	void UpdateView();
	void RefreshWindow() const;

	LRESULT OnSetFont();
	LRESULT OnSize(UINT nFlags, int width, int height);
	LRESULT OnMouseActivate(HWND hwndTop, UINT nHitTest, UINT nMessage)const;
	LRESULT OnPaint();

	LONG GetLeftMargin();

private:
	const HWND m_hWnd;
	COLORREF m_backgroundColor;
	COLORREF m_textColor;
	long m_nFontHeight;
	long m_nLineHeight;
	long m_nFontWidth;
	HFONT m_hFont;
	std::shared_ptr<IStringListModel> m_model;
};

}

#endif
