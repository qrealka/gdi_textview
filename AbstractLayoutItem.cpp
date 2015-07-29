#include "AbstractLayoutItem.h"
#include "IDrawableElement.h"

namespace kofax
{

AbstractLayoutItem::AbstractLayoutItem(const IDrawableElement& layout)
	: AbstractElement(layout.GetOwnerWindow())
{
	m_style = layout.GetStyle();
	layout.GetClientRect(m_ownerRect);
}

}