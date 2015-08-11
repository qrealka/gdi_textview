#include "TextViewStyle.h"

namespace 
{

const UINT TextRenderParams = DT_LEFT | DT_BOTTOM | DT_EXTERNALLEADING | DT_EDITCONTROL | DT_NOPREFIX;

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
		return 0;
	}

	auto hOld = SelectObject(hdc, m_font);

	unsigned flags = 0;
	SIZE sz{ 0, 0 };
	::GetTextExtentPoint32W(hdc, text, textSize, &sz);

	// too long word or too small window - need use slow DrawText decision
	if (rect.left < 5 && sz.cx > rect.right - rect.left)
	{
		RECT rcCalc{ 0, 0, rect.right - rect.left, LONG_MAX };
		flags = TextRenderParams | DT_WORDBREAK;
		DrawText(hdc, const_cast<wchar_t*>(text), textSize, &rcCalc, flags | DT_CALCRECT);
		rect.bottom = rect.top + rcCalc.bottom;
	} 
	else
	{
		rect.right = rect.left + sz.cx;
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

	if (flags)
	{
		DRAWTEXTPARAMS params = { sizeof(DRAWTEXTPARAMS), 1, 0, 0, textSize };
		auto rc = rect;
		DrawTextEx(hdc, const_cast<wchar_t*>(text), textSize, &rc, flags, &params);
	}
	else
	{
		DRAWTEXTPARAMS params = { sizeof(DRAWTEXTPARAMS), 1, 0, 0, textSize };
		auto rc = rect;
		DrawTextEx(hdc, const_cast<wchar_t*>(text), textSize, &rc, TextRenderParams | DT_SINGLELINE | DT_NOCLIP | DT_EXPANDTABS, &params);
	}

	SelectObject(hdc, hOld);
}

void TextViewStyle::PaintBackground(HDC hdc, const RECT& rect) const
{
	if (!m_defaultStyle)
		SetBkColor(hdc, m_backgroundColor);
	ExtTextOut(hdc, 0, 0, ETO_OPAQUE, &rect, nullptr, 0, nullptr);
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