#ifndef _EB17339D_9AB6_4E2B_9D9D_53690DD80760_
#define _EB17339D_9AB6_4E2B_9D9D_53690DD80760_

#include "IStyleView.h"

namespace kofax
{

class TextViewStyle : public IStyleView
{
public:
	explicit TextViewStyle(HWND);
	TextViewStyle(wchar_t* fontName, int fontSize, COLORREF fontColor, COLORREF background);
	~TextViewStyle();

private:
	void SizeText(HDC hdc, RECT& rect, const wchar_t* text, size_t textSize) const override final;
	void PaintText(HDC hdc, const RECT & rect, const wchar_t* text, size_t textSize) const final override;
	void PaintBackground(HDC hdc, int width, long height) const final override;
	void GetFontMetrics(HDC hdc, long& width, long& height) const final override;

private:
	bool m_defaultStyle;
	HFONT m_font;
	COLORREF m_foregroundColor;
	COLORREF m_backgroundColor;
};

}

#endif
