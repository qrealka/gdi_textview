#include "WordWrapLayoutItem.h"
#include "IStackLayoutView.h"
#include "IStyleView.h"

#include <memory>
#include <assert.h>

namespace kofax
{

ILayoutItem* WordWrapLayoutItem::MakeWordWrappedText(const WordWrapLayout& layout,
		const wchar_t* begin, const wchar_t* end, IStyleView* const style)
{
	int x, y;
	layout.GetEndPosition(x, y);
	auto item = std::make_unique<WordWrapLayoutItem>(layout, x, y);
	item->SetStyle(style);
	item->SetDisplayText(begin, end);
	item->Resize();
	return item.release();
}

WordWrapLayoutItem::WordWrapLayoutItem(const IStackLayoutView& layout, int x, int y)
	: m_owner(layout.GetOwnerWindow())
	, m_style(layout.GetStyle())
{
	layout.GetClientRect(m_ownerRect);
	m_clienRect.left = x;
	m_clienRect.top = y;
}

void WordWrapLayoutItem::Resize()
{
	auto hdc = GetDC(m_owner);

	const auto size = m_style->SizeText(hdc, m_displayText, m_displayText + m_displayTextLength);
	ReleaseDC(nullptr, hdc);
	//m_clienRect.right =
	// TODO: wrap  word characters in vertical rectange
}

void WordWrapLayoutItem::SetDisplayText(const wchar_t* begin, const wchar_t* end)
{
	assert(begin <= end);
	if (begin && *begin && begin < end)
	{
		m_displayText = begin;
		m_displayTextLength = end - begin;
	}
	else
	{
		m_displayText = nullptr;
		m_displayTextLength = 0;
	}
}

HWND WordWrapLayoutItem::GetOwnerWindow() const
{
	return m_owner;
}

void WordWrapLayoutItem::SetStyle(IStyleView* const style)
{
	if (style)
		m_style.reset(style);
}

const std::shared_ptr<IStyleView>& WordWrapLayoutItem::GetStyle() const
{
	return m_style;
}

void WordWrapLayoutItem::GetClientRect(RECT& rect) const
{
	rect = m_clienRect;
}

void WordWrapLayoutItem::OnPaint(HDC hdc)
{
}

void WordWrapLayoutItem::OnWindowResize(int width, int height)
{
	m_ownerRect.right = m_ownerRect.left + width;
	m_ownerRect.bottom = m_ownerRect.top + height;
}

}