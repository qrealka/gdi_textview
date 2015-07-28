#include "WordWrapLayoutItem.h"
#include "IStyleView.h"

#include <memory>
#include <assert.h>

namespace kofax
{

AbstractLayoutItem* WordWrapLayoutItem::MakeWordWrappedText(const IStackLayoutView& layout,
	const wchar_t* begin, const wchar_t* end, bool newLine, const std::shared_ptr<IStyleView>& style)
{
	auto item = std::make_unique<WordWrapLayoutItem>(layout, newLine);

	item->SetStyle(style);
	item->SetDisplayText(begin, end);
	
	return item.release();
}

WordWrapLayoutItem::WordWrapLayoutItem(const IStackLayoutView& layout, bool endOfLine)
	: m_owner(layout.GetOwnerWindow())
	, m_style(layout.GetStyle())
	, m_EOL(endOfLine)
{
	layout.GetClientRect(m_ownerRect);
	m_clienRect.left = m_ownerRect.left;
	m_clienRect.top = m_ownerRect.top;
}

void WordWrapLayoutItem::Resize()
{
	if (!m_style)
	{
		m_clienRect.right = m_clienRect.left;
		m_clienRect.bottom = m_clienRect.top;
		return;
	}

	m_clienRect.bottom = LONG_MAX; 
	m_clienRect.right = LONG_MAX; 

	auto hdc = GetDC(nullptr);
	m_style->SizeText(hdc, m_clienRect, m_displayText, m_displayTextLength);
	if (m_clienRect.right > m_ownerRect.right)
	{
		m_clienRect.left = m_ownerRect.left;
		m_clienRect.top = m_clienRect.bottom;
		m_clienRect.bottom = LONG_MAX;
		m_clienRect.right = LONG_MAX;
		m_style->SizeText(hdc, m_clienRect, m_displayText, m_displayTextLength);
	}
	ReleaseDC(nullptr, hdc);
	if (m_EOL)
		m_clienRect.right = m_ownerRect.right;
}

void WordWrapLayoutItem::SetTop(int x, int y)
{
	m_clienRect.left = x;
	m_clienRect.top = y;
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

void WordWrapLayoutItem::OnPaint(HDC hdc)
{
	if (m_style)
	{
		m_style->PaintBackground(hdc, m_clienRect);
		m_style->PaintText(hdc, m_clienRect, m_displayText, m_displayTextLength);
	}
}

void WordWrapLayoutItem::OnWindowResize(int width, int height)
{
	m_ownerRect.right = m_ownerRect.left + width;
	m_ownerRect.bottom = m_ownerRect.top + height;
	Resize();
}

}