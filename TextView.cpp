#include "TextView.h"
#include "IStringListModel.h"
#include "IStringIndex.h"
#include <string>
#include <tchar.h>
#include <assert.h>

namespace
{

std::wstring GetLine(size_t ind)
{
	return std::wstring(L"Hello world ") + std::to_wstring(ind);
}

}

namespace kofax
{

TextView::TextView(HWND hwndParent)	
	: m_hWnd(hwndParent)
	, m_backgroundColor(RGB(128,128,128))
	, m_textColor(RGB(0,0,0))
	, m_hFont(static_cast<HFONT>(GetStockObject(DEFAULT_GUI_FONT)))
{
	OnSetFont();
}

void TextView::SetModel(IStringListModel* const model)
{
	if (model)	{
		m_model.reset(model);
		UpdateView();
	} else {
		assert(false);
	}
}

LONG TextView::GetLeftMargin()
{
	return 5;
}


LRESULT TextView::OnPaint()
{
	auto hrgnUpdate = CreateRectRgn(0, 0, 1, 1);
	GetUpdateRgn(m_hWnd, hrgnUpdate, FALSE);

	PAINTSTRUCT ps;
	BeginPaint(m_hWnd, &ps);

	// current region
	RECT rect;
	GetClientRect(m_hWnd, &rect);

	auto hdcMem = CreateCompatibleDC(ps.hdc);
	auto hbmMem = CreateCompatibleBitmap(ps.hdc, rect.right - rect.left, m_nLineHeight);
	SelectObject(hdcMem, hbmMem);
	size_t last = m_model ? m_model->GetStringsCount() : max(ps.rcPaint.bottom / m_nLineHeight, 0);

	//for (LONG i = 0, last = max(ps.rcPaint.bottom / m_nLineHeight, 0); i <= last; ++i) {
	for (size_t i = 0; i <= last; ++i)
	{
		auto sy = i * m_nLineHeight;
		auto width = rect.right - rect.left;

		PaintBackground(hdcMem, 0, 0, width, m_nLineHeight);
		if (m_model)
		{
			PaintText(hdcMem, m_model->GetString(i), 0, 0, hrgnUpdate);
		}

		// transfer to screen 
		BitBlt(ps.hdc, 0, sy, width, m_nLineHeight, hdcMem, 0, 0, SRCCOPY);
	}

	EndPaint(m_hWnd, &ps);

	DeleteDC(hdcMem);
	DeleteObject(hbmMem);
	DeleteObject(hrgnUpdate);

	return 0;

}

void TextView::RefreshWindow() const
{
	InvalidateRect(m_hWnd, nullptr, FALSE);
}

void TextView::UpdateView()
{
	RECT rect;
	GetClientRect(m_hWnd, &rect);

	OnSize(0, rect.right, rect.bottom);
}

LRESULT TextView::OnSetFont()
{

	auto hdc = GetDC(m_hWnd);
	auto hOld = SelectObject(hdc, m_hFont);

	TEXTMETRIC tm;
	GetTextMetrics(hdc, &tm);

	m_nFontHeight = tm.tmHeight;
	m_nLineHeight = tm.tmHeight + tm.tmExternalLeading;
	m_nFontWidth = tm.tmAveCharWidth;

	SelectObject(hdc, hOld);
	ReleaseDC(m_hWnd, hdc);

	UpdateView();

	return 0;
}

LRESULT TextView::OnSize(UINT nFlags, int width, int height)
{
	RefreshWindow();
	return 0;
}

LRESULT TextView::OnMouseActivate(HWND hwndTop, UINT nHitTest, UINT nMessage)const
{
	SetFocus(m_hWnd);
	return MA_ACTIVATE;
}


const TCHAR* TextView::GetTextViewClassName()
{
	static auto name = _T("TextView");
	return name;
}

void TextView::PaintBackground(HDC hdc, int x, int y, int width, long height)const
{
	const RECT rect = { x, y, x + width, y + height };

	auto fill = SetBkColor(hdc, m_backgroundColor);
	ExtTextOut(hdc, 0, 0, ETO_OPAQUE, &rect, nullptr, 0, nullptr);
	//SetBkColor(hdc, fill);
}

void TextView::PaintText(HDC hdc, const std::unique_ptr<const IStringIndex>& line, int x, int y, HRGN region)const
{
	if (line && line->IsValid())
	{
		RECT bounds;
		HRGN hrgnBounds = nullptr;

		GetClientRect(m_hWnd, &bounds);
		SelectClipRgn(hdc, nullptr);

		SetBkMode(hdc, TRANSPARENT);
		SetTextAlign(hdc, TA_LEFT);
		const auto& str = line->GetLine();

		//SIZE  sz;
		//GetTextExtentPoint32(hdc, str.c_str(), str.length(), &sz);
		//const RECT rc = { x, y, min(x + sz.cx, rect.right), min(y + m_nLineHeight, rect.bottom) };

		ExtTextOut(hdc, x, y, ETO_CLIPPED | ETO_OPAQUE, &bounds, str.c_str(), str.length(), nullptr);

		DeleteObject(hrgnBounds);
		SelectClipRgn(hdc, nullptr);
	}
}

LRESULT TextView::WndProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_PAINT:
		return OnPaint();

	case WM_SETFONT:
		return OnSetFont(/*reinterpret_cast<HFONT>(wParam)*/);

	case WM_SIZE:
		return OnSize(wParam, LOWORD(lParam), HIWORD(lParam));

	case WM_MOUSEACTIVATE:
		return OnMouseActivate(reinterpret_cast<HWND>(wParam), LOWORD(lParam), HIWORD(lParam));

	case WM_SETCURSOR:
		{
			if (LOWORD(lParam) == HTCLIENT)
				return TRUE;
			break;
		}

	default:
		break;
	}

	return DefWindowProc(m_hWnd, msg, wParam, lParam);
}

}