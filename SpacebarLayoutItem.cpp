#include "SpacebarLayoutItem.h"
#include "IStyleView.h"
#include "AbstractStackLayout.h"

#include <assert.h>

namespace kofax
{

AbstractLayoutItem* SpacebarLayoutItem::MakeSpaces(const AbstractStackLayout& layout, 
	const wchar_t* begin, const wchar_t* end, bool newLine, const std::shared_ptr<IStyleView>& style)
{
	assert(begin && end);
	assert(begin < end);

	std::unique_ptr<SpacebarLayoutItem> item(new SpacebarLayoutItem(layout, end - begin, newLine));

	item->SetStyle(style);
	item->SetDisplayText(begin, end);

	return item.release();
}

SpacebarLayoutItem::SpacebarLayoutItem(const IDrawableElement& layout, size_t spaceCount, bool endOfLine)
	: AbstractLayoutItem(layout)
	, m_text(spaceCount, L' ')
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

void SpacebarLayoutItem::SetTop(int x, int y)
{
	m_clientRect.left = x;
	m_clientRect.top = y;
}

void SpacebarLayoutItem::SetDisplayText(const wchar_t*, const wchar_t*)
{}

void SpacebarLayoutItem::OnPaint(HDC hdc)
{
	if (m_style)
		m_style->PaintBackground(hdc, m_clientRect);
}

void SpacebarLayoutItem::OnWindowResize(int width, int height)
{
	m_ownerRect.right = m_ownerRect.left + width;
	m_ownerRect.bottom = m_ownerRect.top + height;

	if (!m_style)
	{
		m_clientRect.right = m_clientRect.left;
		m_clientRect.bottom = m_clientRect.top;
		return;
	}

	m_clientRect.bottom = m_lineHeight ? m_clientRect.top + m_lineHeight : m_ownerRect.bottom;;
	m_clientRect.right = m_ownerRect.right;

	auto hdc = GetDC(nullptr);
	m_style->SizeText(hdc, m_clientRect, m_text.c_str(), m_text.length());

	if (m_clientRect.right > m_ownerRect.right)
	{
		const long spaceWidth = (m_clientRect.right - m_clientRect.left) / m_text.length();
		const long missing = (m_clientRect.right - m_ownerRect.right) / spaceWidth;
		if (missing<2)
		{
			m_clientRect.right = m_ownerRect.right;
		}
		else
		{
			m_clientRect.left = m_ownerRect.left;
			m_clientRect.top = m_clientRect.bottom;
			m_clientRect.bottom = m_lineHeight ? m_clientRect.top + m_lineHeight : m_ownerRect.bottom;
			m_clientRect.right = m_ownerRect.right;
			m_style->SizeText(hdc, m_clientRect, m_text.c_str(), missing);
		}
	}
	ReleaseDC(nullptr, hdc);
	if (m_EOL)
		m_clientRect.right = m_ownerRect.right;
}

}