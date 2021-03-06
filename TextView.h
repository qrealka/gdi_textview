#ifndef _BFB60DF2_ABB4_47F6_95CD_E5EAA5FC02BE_
#define _BFB60DF2_ABB4_47F6_95CD_E5EAA5FC02BE_

#include "AbstractView.h"
#include "AbstractStackLayout.h"
#include <memory>

namespace kofax
{

struct IListModel;
struct IListIndex;

class TextView : public AbstractView
{
public:
	explicit TextView(HWND hwndParent);

private:
	// IDrawableItem
	void OnPaint(HDC hdc) final override;
	void OnWindowResize(int width, int height) final override;

private:
	// AbstractView
	void UpdateView() final override;
	void SetLayout(AbstractStackLayout* const layout) final override;

	LRESULT OnKeyDown(WPARAM keyCode, LPARAM flags);
	LRESULT WndProc(UINT msg, WPARAM wParam, LPARAM lParam) final override;

	void RefreshWindow() const;

	LRESULT OnMouseActivate(HWND hwndTop, UINT nHitTest, UINT nMessage)const;
	LRESULT OnPaint();

private:
	std::unique_ptr<AbstractStackLayout> m_layoutText;
};

}

#endif
