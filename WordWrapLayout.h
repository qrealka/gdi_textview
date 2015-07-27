#ifndef _78126AE2_660E_4AB2_AAA5_B5068B61ED42_
#define _78126AE2_660E_4AB2_AAA5_B5068B61ED42_

#include "IStackLayoutView.h"
#include <list>

namespace kofax
{

class WordWrapLayout : public IStackLayoutView
{
public:
	explicit WordWrapLayout(const IDrawableElement& owner, const RECT& clientRect);

private:
	// IStackLayoutView
	//void GetEndPosition(int& x, int& y) const final override;
	void Clear() final override;
	bool ItemPop() final override;
	void ItemPush(ILayoutItem* const item) final override;

	// IDrawableElement
	HWND GetOwnerWindow() const final override;
		
	void SetStyle(std::shared_ptr<IStyleView>style) final override;
	const std::shared_ptr<IStyleView>& GetStyle() const final override;

	void GetClientRect(RECT& rect) const final override;
	void OnPaint(HDC hdc) final override;
	void OnWindowResize(int width, int height) final override;

private:
	typedef std::unique_ptr<ILayoutItem> LayoutItem;
	typedef std::list<LayoutItem> StackItems;

	HWND m_owner;
	RECT m_clientRect;
	std::shared_ptr<IStyleView> m_style;
	StackItems m_items;
	int  m_lastX;
	int  m_lastY;
};

}

#endif
