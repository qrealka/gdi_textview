#include "WordWrapLayoutItem.h"
#include "IStyleView.h"
#include "AbstractStackLayout.h"

#include <memory>
#include <assert.h>

namespace kofax
{

AbstractLayoutItem* WordWrapLayoutItem::MakeWordWrappedText(const AbstractStackLayout& layout,
	const wchar_t* begin, const wchar_t* end, bool newLine, const std::shared_ptr<IStyleView>& style)
{
	std::unique_ptr<WordWrapLayoutItem> item(new WordWrapLayoutItem(layout, newLine));

	item->SetStyle(style);
	item->SetDisplayText(begin, end);
	
	return item.release();
}

WordWrapLayoutItem::WordWrapLayoutItem(const IDrawableElement& layout, bool endOfLine)
	: AbstractLayoutItem(layout) 
	, m_EOL(endOfLine)
{
	m_clientRect.left = m_ownerRect.left;
	m_clientRect.top = m_ownerRect.top;
}

void WordWrapLayoutItem::Resize()
{
	if (!m_style)
	{
		m_clientRect.right = m_clientRect.left;
		m_clientRect.bottom = m_clientRect.top;
		return;
	}

	m_clientRect.bottom = LONG_MAX;
	m_clientRect.right = LONG_MAX;

	auto hdc = GetDC(nullptr);
	m_style->SizeText(hdc, m_clientRect, m_displayText, m_displayTextLength);
	if (m_clientRect.right > m_ownerRect.right)
	{
		m_clientRect.left = m_ownerRect.left;
		m_clientRect.top = m_clientRect.bottom;
		m_clientRect.bottom = LONG_MAX;
		m_clientRect.right = LONG_MAX;
		m_style->SizeText(hdc, m_clientRect, m_displayText, m_displayTextLength);
	}
	ReleaseDC(nullptr, hdc);
	if (m_EOL)
		m_clientRect.right = m_ownerRect.right;
}

void WordWrapLayoutItem::SetTop(int x, int y)
{
	m_clientRect.left = x;
	m_clientRect.top = y;
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
		m_style->PaintBackground(hdc, m_clientRect);
		m_style->PaintText(hdc, m_clientRect, m_displayText, m_displayTextLength);
	}
}

void WordWrapLayoutItem::OnWindowResize(int width, int height)
{
	m_ownerRect.right = m_ownerRect.left + width;
	m_ownerRect.bottom = m_ownerRect.top + height;
	Resize();
}

}