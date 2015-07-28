#include "WordWrapLayout.h"
#include "ILayoutItem.h"
#include "IStyleView.h"
#include "IItemDelegate.h"

#include <algorithm>

namespace kofax
{

WordWrapLayout::WordWrapLayout(const IDrawableElement& owner, const RECT& clientRect, IItemDelegate* const itemDelegate)
	: m_owner(owner.GetOwnerWindow())
	, m_clientRect(clientRect)
	, m_style(owner.GetStyle())
	, m_itemDelegate(itemDelegate)
	, m_lastX(clientRect.left)
	, m_lastY(clientRect.top)
{
}


void WordWrapLayout::Clear()
{
	m_items.clear();
}

bool WordWrapLayout::ItemPop()
{
	if (m_items.empty())
	{
		m_lastX = m_clientRect.left;
		m_lastY = m_clientRect.top;
		return false;
	}

	m_items.pop_back();
	if (m_items.empty())
	{
		m_lastX = m_clientRect.left;
		m_lastY = m_clientRect.top;
		return true;
	}

	RECT rect;
	m_items.back()->GetClientRect(rect);
	m_lastY = rect.bottom;
	m_lastX = rect.right;

	return true;
}

void WordWrapLayout::ItemPush(ILayoutItem* const item)
{
	if (item)
	{
		item->SetTop(m_lastX, m_lastY);
		item->OnWindowResize(m_clientRect.right - m_clientRect.left,
			m_clientRect.bottom - m_clientRect.top);

		RECT rect;
		item->GetClientRect(rect);

		if (rect.right > m_clientRect.right)
		{
			item->SetTop(m_clientRect.left, rect.bottom);
			item->OnWindowResize(m_clientRect.right - m_clientRect.left,
				m_clientRect.bottom - m_clientRect.top);
		}

		m_lastY = rect.bottom;
		m_lastX = rect.right;

		m_items.emplace_back(item);
	}
}

void WordWrapLayout::AddSpaces(ILayoutItem* const item)
{
}

HWND WordWrapLayout::GetOwnerWindow() const
{
	return m_owner;
}

void WordWrapLayout::SetStyle(std::shared_ptr<IStyleView>style)
{
	if (style)
		m_style = std::move(style);
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
	m_style->PaintBackground(hdc, m_clientRect.right, m_clientRect.bottom);

	std::find_if(cbegin(m_items), cend(m_items), [&hdc, this](const std::unique_ptr<ILayoutItem>& item)
	{
		RECT rect;
		item->GetClientRect(rect);
		if (rect.top > m_clientRect.bottom)
			return true;

		item->OnPaint(hdc);
		return false;
	});
}

void WordWrapLayout::OnWindowResize(int width, int height)
{
	m_clientRect.right = m_clientRect.left + width;
	m_clientRect.bottom = m_clientRect.top + height;
	if (m_itemDelegate)
		m_itemDelegate->OnWindowResize(width, height, *this);
}

}