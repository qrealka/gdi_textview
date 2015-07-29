#ifndef _78126AE2_660E_4AB2_AAA5_B5068B61ED42_
#define _78126AE2_660E_4AB2_AAA5_B5068B61ED42_

#include "AbstractStackLayout.h"
#include <list>

namespace kofax
{

struct IItemDelegate;

class WordWrapLayout : public AbstractStackLayout
{
public:
	WordWrapLayout(const IDrawableElement& owner, 
		const RECT& clientRect, IItemDelegate* const itemDelegate);

private:
	// AbstractStackLayout
	void Clear() final override;
	bool ItemPop() final override;
	void ItemPush(AbstractLayoutItem* const item) final override;

	void OnPaint(HDC hdc) final override;
	void OnWindowResize(int width, int height) final override;

private:
	typedef std::unique_ptr<AbstractLayoutItem> LayoutItem;
	typedef std::list<LayoutItem> StackItems;

	std::unique_ptr<IItemDelegate> m_itemDelegate;
	StackItems m_items;
	int  m_lastX;
	int  m_lastY;
};

}

#endif
