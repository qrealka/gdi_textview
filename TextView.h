#ifndef _BFB60DF2_ABB4_47F6_95CD_E5EAA5FC02BE_
#define _BFB60DF2_ABB4_47F6_95CD_E5EAA5FC02BE_

#include "IListView.h"
#include "IStackLayoutView.h"
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
	// IDrawableItem
	HWND GetOwnerWindow() const final override;
	void SetStyle(IStyleView* const style) final override;
	const std::shared_ptr<IStyleView>& GetStyle() const final override;
	void GetClientRect(RECT& rect) const final override;
	void OnPaint(HDC hdc) final override;
	void OnWindowResize(int width, int height) final override;

private:
	void SetModel(const std::shared_ptr<IListModel>& model) final override;
	void SetLayout(IStackLayoutView* const layout) final override;
	LRESULT WndProc(UINT msg, WPARAM wParam, LPARAM lParam) final override;

	void PaintText(HDC hdc, std::unique_ptr<const IListIndex> line)const;

	void UpdateView();
	void RefreshWindow() const;

	LRESULT OnMouseActivate(HWND hwndTop, UINT nHitTest, UINT nMessage)const;
	LRESULT OnPaint();

	LONG GetLeftMargin();

private:
	HWND m_hWnd;
	long m_nLineHeight;
	long m_nFontWidth;
	std::shared_ptr<IStyleView> m_style;
	std::shared_ptr<IListModel> m_model;
	std::unique_ptr<IStackLayoutView> m_layoutText;
};

}

#endif
