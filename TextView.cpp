#include "TextView.h"
#include "IListModel.h"
#include "IListIndex.h"
#include "IStackLayoutView.h"
#include <string>
#include <assert.h>

namespace kofax
{

TextView::TextView(HWND hwndParent)	
	: m_hWnd(hwndParent)
{
}

HWND TextView::GetOwnerWindow() const
{
	return m_hWnd;
}

void TextView::SetStyle(IStyleView* const style)
{
	if (style)
		m_style.reset(style);
}

const std::shared_ptr<IStyleView>& TextView::GetStyle() const
{
	return m_style;
}

void TextView::GetClientRect(RECT& rect) const
{
	::GetClientRect(m_hWnd, &rect);
}

void TextView::OnPaint(HDC hdc)
{
	m_layoutText->OnPaint(hdc);
}

void TextView::OnWindowResize(int width, int height)
{
	m_layoutText->OnWindowResize(width, height);
	RefreshWindow();
}

void TextView::SetModel(const std::shared_ptr<IListModel>& model)
{
	if (model)	{
		m_model = model;
		UpdateView();
	} else {
		assert(false);
	}
}

void TextView::SetLayout(IStackLayoutView* const layout)
{
	if (layout)
	{
		m_layoutText.reset(layout);
		UpdateView();
	}
}

/*void TextView::SetStyle(IStyleView* const style)
{
	if (style) {
		m_defaultStyle.reset(style);
		auto hdc = GetDC(m_hWnd);

		m_defaultStyle->GetFontMetrics(hdc, m_nFontWidth, m_nLineHeight);

		ReleaseDC(m_hWnd, hdc);
		UpdateView();
	} else {
		assert(false);
	}
}*/

LONG TextView::GetLeftMargin()
{
	return 5;
}


LRESULT TextView::OnPaint()
{
	//auto hrgnUpdate = CreateRectRgn(0, 0, 1, 1);
	//GetUpdateRgn(m_hWnd, hrgnUpdate, FALSE);

	PAINTSTRUCT ps;
	BeginPaint(m_hWnd, &ps);
	OnPaint(ps.hdc);
	// current region
	/*RECT rect;
	GetClientRect(m_hWnd, &rect);

	auto hdcMem = CreateCompatibleDC(ps.hdc);
	auto hbmMem = CreateCompatibleBitmap(ps.hdc, rect.right - rect.left, m_nLineHeight);
	SelectObject(hdcMem, hbmMem);
	size_t last = max(ps.rcPaint.bottom / m_nLineHeight, 0);

	for (size_t i = 0; i <= last; ++i)
	{
		auto sy = i * m_nLineHeight;
		auto width = rect.right - rect.left;

		m_defaultStyle->PaintBackground(hdcMem, width, m_nLineHeight);
		//if (m_model && i < m_model->GetSize())
		//	PaintText(hdcMem, m_model->GetIndex(i));

		// transfer to screen 
		BitBlt(ps.hdc, 0, sy, width, m_nLineHeight, hdcMem, 0, 0, SRCCOPY);
	}
	*/

	EndPaint(m_hWnd, &ps);
	/*
	DeleteDC(hdcMem);
	DeleteObject(hbmMem);
	DeleteObject(hrgnUpdate);
	*/
	return 0;

}

void TextView::RefreshWindow() const
{
	InvalidateRect(m_hWnd, nullptr, FALSE);
}

void TextView::UpdateView()
{
	RECT rect;
	::GetClientRect(m_hWnd, &rect);

	OnWindowResize(rect.right, rect.bottom);
}


LRESULT TextView::OnMouseActivate(HWND hwndTop, UINT nHitTest, UINT nMessage) const
{
	SetFocus(m_hWnd);
	return MA_ACTIVATE;
}


void TextView::PaintText(HDC hdc, std::unique_ptr<const IListIndex> line) const
{
	if (line && line->IsValid())
	{
		RECT bounds;
		::GetClientRect(m_hWnd, &bounds);

		const auto& str = line->ToString();
		//m_defaultStyle->PaintText(hdc, bounds.right, m_nLineHeight, str.c_str(), str.c_str() + str.length());
	}
}

LRESULT TextView::WndProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_PAINT:
		return OnPaint();

	case WM_SIZE:
		OnWindowResize(LOWORD(lParam), HIWORD(lParam));
		return 0;

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