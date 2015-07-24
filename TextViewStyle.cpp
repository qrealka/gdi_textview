#include "TextViewStyle.h"
#include <assert.h>

namespace 
{

int PointsToLogical(int pointSize)
{
	auto hdc = GetDC(nullptr);
	auto fontSize = -MulDiv(pointSize, GetDeviceCaps(hdc, LOGPIXELSY), 72);
	ReleaseDC(nullptr, hdc);

	return fontSize;
}

HFONT EasyCreateFont(wchar_t *szFace, int pointSize)
{
	return CreateFont(PointsToLogical(pointSize),
		0, 0, 0,
		0,
		0, 0, 0, DEFAULT_CHARSET, 0, 0,
		DEFAULT_QUALITY,
		0,
		szFace);
}

}

namespace kofax
{

TextViewStyle::TextViewStyle(wchar_t* fontName, int fontSize, COLORREF fontColor, COLORREF background)
	: m_font(EasyCreateFont(fontName, fontSize))
	, m_foregroundColor(fontColor)
	, m_backgroundColor(background)
{
	
}

TextViewStyle::~TextViewStyle()
{
	DeleteObject(m_font);
}

void TextViewStyle::PaintText(HDC hdc, int width, long height, const wchar_t* begin, const wchar_t* end) const
{
	assert(begin && end);
	assert(begin <= end);

	const auto length = end - begin;
	auto hOld = SelectObject(hdc, m_font);

	SetBkMode(hdc, TRANSPARENT);
	SetTextAlign(hdc, TA_LEFT);

	SIZE  sz;
	GetTextExtentPoint32(hdc, begin, length, &sz);
	const RECT bounds = { 0, 0, min(sz.cx, width), min(sz.cy, height) };

	ExtTextOut(hdc, 0, 0, ETO_CLIPPED | ETO_OPAQUE, &bounds, begin, length, nullptr);
	SelectObject(hdc, hOld);
}

void TextViewStyle::PaintBackground(HDC hdc, int width, long height) const
{
	const RECT rect = { 0, 0, width, height };

	auto fill = SetBkColor(hdc, m_backgroundColor);
	ExtTextOut(hdc, 0, 0, ETO_OPAQUE, &rect, nullptr, 0, nullptr);
	//SetBkColor(hdc, fill);
}

void TextViewStyle::GetFontMetrics(HDC hdc, long& width, long& height) const
{
	auto hOld = SelectObject(hdc, m_font);

	TEXTMETRIC tm;
	GetTextMetrics(hdc, &tm);

	height = tm.tmHeight + tm.tmExternalLeading;
	width = tm.tmAveCharWidth;

	SelectObject(hdc, hOld);
}

}