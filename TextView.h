#ifndef _BFB60DF2_ABB4_47F6_95CD_E5EAA5FC02BE_
#define _BFB60DF2_ABB4_47F6_95CD_E5EAA5FC02BE_

#include "IListView.h"
#include "IStyleView.h"
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
	void SetModel(const std::shared_ptr<IListModel>& model) final override;
	void SetStyle(IStyleView* const style) final override;
	LRESULT WndProc(UINT msg, WPARAM wParam, LPARAM lParam) final override;

	void PaintText(HDC hdc, const std::unique_ptr<const IListIndex>& line)const;

	void UpdateView();
	void RefreshWindow() const;

	LRESULT OnSize(UINT nFlags, int width, int height);
	LRESULT OnMouseActivate(HWND hwndTop, UINT nHitTest, UINT nMessage)const;
	LRESULT OnPaint();

	LONG GetLeftMargin();

private:
	const HWND m_hWnd;
	long m_nLineHeight;
	long m_nFontWidth;
	std::shared_ptr<IListModel> m_model;
	std::unique_ptr<IStyleView> m_defaultStyle;
};

}

#endif
