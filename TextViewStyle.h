#ifndef _EB17339D_9AB6_4E2B_9D9D_53690DD80760_
#define _EB17339D_9AB6_4E2B_9D9D_53690DD80760_

#include "IStyleView.h"

namespace kofax
{

class TextViewStyle : public IStyleView
{
public:
	TextViewStyle(wchar_t* fontName, int fontSize, COLORREF fontColor, COLORREF background);
	~TextViewStyle();

private:
	SIZE SizeText(HDC hdc, const wchar_t* begin, const wchar_t* end) const final override;
	void PaintText(HDC hdc, int width, long height, const wchar_t* begin, const wchar_t* end) const final override;
	void PaintBackground(HDC hdc, int width, long height) const final override;
	void GetFontMetrics(HDC hdc, long& width, long& height) const final override;

private:
	HFONT m_font;
	COLORREF m_foregroundColor;
	COLORREF m_backgroundColor;
};

}

#endif
