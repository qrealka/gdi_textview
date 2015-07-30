#ifndef _031D915A_C043_4FEE_B6B1_F856B9A3B58F_
#define _031D915A_C043_4FEE_B6B1_F856B9A3B58F_

#include "AbstractElement.h"

namespace kofax
{

struct AbstractLayoutItem;

struct AbstractStackLayout : public AbstractElement
{
	virtual void Clear() = 0;
	virtual bool ItemPop() = 0;
	virtual void ItemPush(AbstractLayoutItem* const item) = 0;
	virtual void ScrollLineUp() = 0;
	virtual void ScrollLineDown() = 0;
protected:
	explicit AbstractStackLayout(const IDrawableElement& owner);
};

}

#endif
