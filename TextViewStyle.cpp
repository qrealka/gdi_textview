#include "TextViewStyle.h"

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

TextViewStyle::TextViewStyle(HWND)
	: m_defaultStyle(true)
	, m_font(static_cast<HFONT>(GetStockObject(ANSI_FIXED_FONT)))
{
	
}

TextViewStyle::TextViewStyle(wchar_t* fontName, int fontSize, COLORREF fontColor, COLORREF background)
	: m_defaultStyle(false)
	, m_font(EasyCreateFont(fontName, fontSize))
	, m_foregroundColor(fontColor)
	, m_backgroundColor(background)
{
	
}

TextViewStyle::~TextViewStyle()
{
	if (!m_defaultStyle)
		DeleteObject(m_font);
}

void TextViewStyle::SizeText(HDC hdc, RECT& rect, const wchar_t* text, size_t textSize) const
{
	if (text == nullptr || !textSize)
	{
		SetRectEmpty(&rect);
		return;
	}

	RECT rcCalc = { 0, 0, rect.right, rect.bottom };

	auto hOld = SelectObject(hdc, m_font);

	DRAWTEXTPARAMS params = { sizeof(DRAWTEXTPARAMS), 4, 0, 0, textSize };
	DrawTextEx(hdc, const_cast<wchar_t*>(text), textSize, &rcCalc, DT_LEFT | DT_WORDBREAK | DT_EDITCONTROL | DT_CALCRECT, &params);
	rect.right = rect.left + rcCalc.right;
	rect.bottom = rect.top + rcCalc.bottom;

	SelectObject(hdc, hOld);
}

void TextViewStyle::PaintText(HDC hdc, const RECT& rect, const wchar_t* text, size_t textSize) const
{
	if (text == nullptr || !textSize)
		return;

	auto hOld = SelectObject(hdc, m_font);

	if (!m_defaultStyle)
	{
		SetBkMode(hdc, TRANSPARENT);
		SetTextAlign(hdc, TA_LEFT); // don't use TA_UPDATECP   
		SetTextColor(hdc, m_foregroundColor);
	}

	auto clientRect = rect;
	SizeText(hdc, clientRect, text, textSize);

	//clientRect.left = rect.left;
	//clientRect.right = rect.right;
	
	DRAWTEXTPARAMS params = { sizeof(DRAWTEXTPARAMS), 4, 0, 0, textSize };
	DrawTextEx(hdc, const_cast<wchar_t*>(text), textSize, &clientRect, DT_LEFT | DT_WORDBREAK | DT_EDITCONTROL, &params);

	SelectObject(hdc, hOld);
}

void TextViewStyle::PaintBackground(HDC hdc, int width, long height) const
{
	const RECT rect = { 0, 0, width, height };

	if (!m_defaultStyle)
		SetBkColor(hdc, m_backgroundColor);
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