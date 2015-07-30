#include "AbstractLayoutItem.h"
#include "IDrawableElement.h"

namespace kofax
{

AbstractLayoutItem::AbstractLayoutItem(const IDrawableElement& layout)
	: AbstractElement(layout.GetOwnerWindow())
	, m_scrollOffset(0)
{
	m_style = layout.GetStyle();
	layout.GetClientRect(m_ownerRect);
}

void AbstractLayoutItem::SetScrollOffset(long offset)
{
	const long delta = offset - m_scrollOffset;
	m_clientRect.top -= delta;
	m_clientRect.bottom -= delta;

	m_scrollOffset = offset;
}

}