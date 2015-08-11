#include "WordWrapLayout.h"
#include "AbstractLayoutItem.h"
#include "IStyleView.h"
#include "IItemDelegate.h"

#include <algorithm>

namespace kofax
{

WordWrapLayout::WordWrapLayout(const IDrawableElement& owner, const RECT& clientRect, IItemDelegate* const itemDelegate)
	: AbstractStackLayout(owner)
	, m_itemDelegate(itemDelegate)
	, m_lastX(clientRect.left)
	, m_lastY(clientRect.top)
	, m_currentLine(0)
	, m_lineHeight(0)
{
	m_clientRect = clientRect;
	m_style = owner.GetStyle();
	if (m_style)
	{
		auto hdc = GetDC(nullptr);
		long width;
		m_style->GetFontMetrics(hdc, width, m_lineHeight);
		ReleaseDC(nullptr, hdc);
	}
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

void WordWrapLayout::AlignItem(AbstractLayoutItem* const item)
{
	item->SetTop(m_lastX, m_lastY);
	item->OnWindowResize(m_clientRect.right - m_clientRect.left,
	                     m_clientRect.bottom - m_clientRect.top);

	RECT rect;
	item->GetClientRect(rect);

	if (rect.right >= m_clientRect.right)
	{
		m_lastY = rect.bottom;
		m_lastX = m_clientRect.left;
	}
	else
	{
		m_lastY = rect.top;
		m_lastX = rect.right;
	}
}

void WordWrapLayout::ItemPush(AbstractLayoutItem* const item)
{
	if (item)
	{
		item->SetScrollOffset(m_currentLine * m_lineHeight);
		AlignItem(item);
		m_items.emplace_back(item);
	}
}

void WordWrapLayout::ScrollLineUp()
{
	if (m_currentLine)
	{
		--m_currentLine;
		for_each(begin(m_items), end(m_items), [this](LayoutItem& item)
		{
			item->SetScrollOffset(m_currentLine * m_lineHeight);
		});
	}
}

void WordWrapLayout::ScrollLineDown()
{
	RECT rect;
	m_items.back()->GetClientRect(rect);
	if (rect.bottom > 0)
	{
		++m_currentLine;
		for_each(begin(m_items), end(m_items), [this](LayoutItem& item)
		{
			item->SetScrollOffset(m_currentLine * m_lineHeight);
		});
	}
}

void WordWrapLayout::OnPaint(HDC hdc)
{
	if (m_style)
	{
		m_style->PaintBackground(hdc, m_clientRect);
	}

	std::find_if(cbegin(m_items), cend(m_items), [&hdc, this](const std::unique_ptr<AbstractLayoutItem>& item)
	{
		RECT rect;
		item->GetClientRect(rect);
		if (rect.bottom <= 0)
			return false;

		if (rect.bottom > m_clientRect.bottom)
			return true;

		item->OnPaint(hdc);
		return false;
	});
}

void WordWrapLayout::OnWindowResize(int width, int height)
{
	m_lastX = m_clientRect.left;
	m_lastY = m_clientRect.top;

	m_clientRect.right = m_clientRect.left + width;
	m_clientRect.bottom = m_clientRect.top + height;

	if (m_itemDelegate && m_items.empty())
	{
		m_itemDelegate->OnWindowResize(width, height, *this);
	}
	else
	{
		m_lastX = m_clientRect.left;
		m_lastY = m_clientRect.top;

		for_each(begin(m_items), end(m_items), [this](std::unique_ptr<AbstractLayoutItem>& item)
		{
			AlignItem(item.get());
		});
	}
}

}