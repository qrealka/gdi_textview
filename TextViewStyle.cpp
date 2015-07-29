#include "TextViewStyle.h"

namespace 
{

	const UINT TextRenderParams = DT_LEFT | DT_BOTTOM | DT_EXTERNALLEADING | DT_EDITCONTROL | DT_EXPANDTABS | DT_NOPREFIX;

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
		PROOF_QUALITY,
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

unsigned TextViewStyle::SizeText(HDC hdc, RECT& rect, const wchar_t* text, size_t textSize) const
{
	if (text == nullptr || !textSize)
	{
		SetRectEmpty(&rect);
		return TextRenderParams | DT_WORDBREAK;;
	}

	RECT rcCalc = { 0, 0, LONG_MAX, LONG_MAX };
	auto hOld = SelectObject(hdc, m_font);

	unsigned flags = TextRenderParams | DT_SINGLELINE;
	DRAWTEXTPARAMS params = { sizeof(DRAWTEXTPARAMS), 4, 0, 0, textSize };
	DrawTextEx(hdc, const_cast<wchar_t*>(text), textSize, &rcCalc, flags | DT_CALCRECT, &params);

	if (rect.left < 5 && rcCalc.right > rect.right)
	{
		// too long word or too small window
		flags = TextRenderParams | DT_WORDBREAK;
		DrawTextEx(hdc, const_cast<wchar_t*>(text), textSize, &rcCalc, TextRenderParams | DT_CALCRECT, &params);
		
		rect.right = rect.left + rcCalc.right;
		rect.bottom = rect.top + rcCalc.bottom; // we don't need vertical algment because GDI ignore it whe DT_WORDBREAK specified
	} 
	else
	{
		rect.right = rect.left + rcCalc.right;
		// if word in one line - use rect.bottom
	}


	SelectObject(hdc, hOld);
	return flags;
}

void TextViewStyle::PaintText(HDC hdc, const RECT& rect, const wchar_t* text, size_t textSize, unsigned flags) const
{
	if (text == nullptr || !textSize)
		return;

	auto hOld = SelectObject(hdc, m_font);

	if (!m_defaultStyle)
	{
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, m_foregroundColor);
	}

	DRAWTEXTPARAMS params = { sizeof(DRAWTEXTPARAMS), 4, 0, 0, textSize };
	DrawTextEx(hdc, const_cast<wchar_t*>(text), textSize, const_cast<RECT*>(&rect), flags, &params);

	SelectObject(hdc, hOld);
}

void TextViewStyle::PaintBackground(HDC hdc, const RECT& rect) const
{
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