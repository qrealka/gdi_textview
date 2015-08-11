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

void AbstractLayoutItem::SetTop(int x, int y)
{
	const auto deltaX = x - m_clientRect.left;
	const auto deltaY = y - m_clientRect.top;

	m_clientRect.left += deltaX;
	m_clientRect.right += deltaX;
	m_clientRect.top += deltaY;
	m_clientRect.bottom += deltaY;
}

void AbstractLayoutItem::SetScrollOffset(long offset)
{
	m_scrollOffset = offset;
}

void AbstractLayoutItem::GetClientRect(RECT& rect) const
{
	rect = m_clientRect;
	rect.top -= m_scrollOffset;
	rect.bottom -= m_scrollOffset;
}

}