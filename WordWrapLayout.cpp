#include "WordWrapLayout.h"
#include "ILayoutItem.h"
#include "IStyleView.h"

#include <algorithm>

namespace kofax
{

WordWrapLayout::WordWrapLayout(const IDrawableElement& owner, const RECT& clientRect)
	: m_owner(owner.GetOwnerWindow())
	, m_clientRect(clientRect)
	, m_style(owner.GetStyle())
	, m_lastX(clientRect.left)
	, m_lastY(clientRect.top)
{
}

void WordWrapLayout::GetEndPosition(int& x, int& y) const
{
	x = m_lastX;
	y = m_lastY;
}

void WordWrapLayout::Clear()
{
	StackItems tmp;
	m_items.swap(tmp);
}

bool WordWrapLayout::ItemPop()
{

	if (m_items.empty())
	{
		return false;
	}

	m_items.pop_back();
	return true;
}

size_t WordWrapLayout::ItemPush(ILayoutItem* const item)
{
	m_items.emplace_back(item);
	return m_items.size() - 1;
}

HWND WordWrapLayout::GetOwnerWindow() const
{
	return m_owner;
}

void WordWrapLayout::SetStyle(IStyleView* const style)
{
	if (style)
		m_style.reset(style);
}

const std::shared_ptr<IStyleView>& WordWrapLayout::GetStyle() const
{
	return m_style;
}

void WordWrapLayout::GetClientRect(RECT& rect) const
{
	rect = m_clientRect;
}

void WordWrapLayout::OnPaint(HDC hdc)
{
	std::for_each(cbegin(m_items), cend(m_items), [&hdc](const std::unique_ptr<ILayoutItem>& item)
	{
		item->OnPaint(hdc);
	});
}

void WordWrapLayout::OnWindowResize(int width, int height)
{
	m_clientRect.right = m_clientRect.left + width;
	m_clientRect.bottom = m_clientRect.top + height;
}

}