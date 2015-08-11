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
	, m_textFlags(0)
	, m_EOL(endOfLine)
{
	m_clientRect.left = m_ownerRect.left;
	m_clientRect.top = m_ownerRect.top;

	if (layout.GetStyle())
	{
		auto hdc = GetDC(nullptr);
		long width;
		layout.GetStyle()->GetFontMetrics(hdc, width, m_lineHeight);
		ReleaseDC(nullptr, hdc);
	}
}

void WordWrapLayoutItem::Resize()
{
	if (!m_style)
	{
		m_clientRect.right = m_clientRect.left;
		m_clientRect.bottom = m_clientRect.top;
		return;
	}

	m_clientRect.bottom = m_lineHeight ? m_clientRect.top + m_lineHeight : m_ownerRect.bottom;
	m_clientRect.right = m_ownerRect.right;

	auto hdc = GetDC(nullptr);
	m_textFlags = m_style->SizeText(hdc, m_clientRect, m_displayText, m_displayTextLength);

	if (m_clientRect.right > m_ownerRect.right)
	{
		// resize very long word and align bottom for scroll
		m_clientRect.left = m_ownerRect.left;
		m_clientRect.top = m_clientRect.bottom;
		m_clientRect.bottom = m_lineHeight ? m_clientRect.top + m_lineHeight : m_ownerRect.bottom;
		m_clientRect.right = m_ownerRect.right;
		m_textFlags = m_style->SizeText(hdc, m_clientRect, m_displayText, m_displayTextLength);
	}

	if (m_lineHeight && m_textFlags)
	{
		const auto lines = (m_clientRect.bottom - m_clientRect.top) / m_lineHeight;
		m_clientRect.bottom = lines * m_lineHeight < m_clientRect.bottom - m_clientRect.top
			? m_clientRect.top + (lines + 1) * m_lineHeight
			: m_clientRect.top + lines * m_lineHeight;
	}
	ReleaseDC(nullptr, hdc);
	if (m_EOL)
		m_clientRect.right = m_ownerRect.right;
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
		auto rect = m_clientRect;
		rect.top -= m_scrollOffset;
		rect.bottom -= m_scrollOffset;

		m_style->PaintBackground(hdc, rect);
		m_style->PaintText(hdc, rect, m_displayText, m_displayTextLength, m_textFlags);
	}
}

void WordWrapLayoutItem::OnWindowResize(int width, int height)
{
	m_ownerRect.right = m_ownerRect.left + width;
	m_ownerRect.bottom = m_ownerRect.top + height;
	Resize();
}

}