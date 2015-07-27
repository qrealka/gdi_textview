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


void WordWrapLayout::Clear()
{
	StackItems tmp;
	m_items.swap(tmp);
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

		long fontHeight, fontWidth;
		auto hdc = GetDC(nullptr);
		item->GetStyle()->GetFontMetrics(hdc, fontWidth, fontHeight);
		ReleaseDC(nullptr, hdc);

		if (fontWidth > (m_clientRect.right - rect.right))
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