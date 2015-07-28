#ifndef _031D915A_C043_4FEE_B6B1_F856B9A3B58F_
#define _031D915A_C043_4FEE_B6B1_F856B9A3B58F_

#include "IDrawableElement.h"

namespace kofax
{

struct IStyleView;
struct AbstractLayoutItem;

struct IStackLayoutView : public IDrawableElement
{
	virtual void Clear() = 0;
	virtual bool ItemPop() = 0;
	virtual void ItemPush(AbstractLayoutItem* const item) = 0;
	virtual void AddSpaces(AbstractLayoutItem* const item) = 0;

protected:
	IStackLayoutView() = default;

private:
	IStackLayoutView(const IStackLayoutView&) = delete;
	IStackLayoutView& operator=(const IStackLayoutView&) = delete;
};

}

#endif
