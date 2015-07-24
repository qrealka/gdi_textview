#ifndef _BFB60DF2_ABB4_47F6_95CD_E5EAA5FC02BE_
#define _BFB60DF2_ABB4_47F6_95CD_E5EAA5FC02BE_

#include "IListView.h"
#include <memory>

namespace kofax
{

struct IListModel;
struct IListIndex;

class TextView : public IListView
{
public:
	explicit TextView(HWND hwndParent);

private:
	TextView(const TextView&) = delete;
	TextView& operator=(const TextView&) = delete;

	void SetModel(const std::shared_ptr<IListModel>& model) final override;
	LRESULT WndProc(UINT msg, WPARAM wParam, LPARAM lParam) final override;

	void PaintBackground(HDC hdc, int x, int y, int width, long height)const;
	void PaintText(HDC hdc, const std::unique_ptr<const IListIndex>& line, int x, int y, HRGN region)const;

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
	std::shared_ptr<IListModel> m_model;
};

}

#endif
