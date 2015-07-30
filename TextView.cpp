#include "TextView.h"
#include "AbstractStackLayout.h"
#include "IStyleView.h"

namespace kofax
{

TextView::TextView(HWND hwndParent)	
	: AbstractView(hwndParent)
{
}


void TextView::OnPaint(HDC hdc)
{
	if (m_layoutText)
	{
		m_layoutText->OnPaint(hdc);
	} 
	else if (m_style)
	{
		RECT rc;
		::GetClientRect(m_owner, &rc);
		m_style->PaintBackground(hdc, rc);
	}
}

void TextView::OnWindowResize(int width, int height)
{
	if (m_layoutText)
		m_layoutText->OnWindowResize(width, height);

	RefreshWindow();
}


void TextView::SetLayout(AbstractStackLayout* const layout)
{
	m_layoutText.reset(layout);
}


LRESULT TextView::OnKeyDown(WPARAM keyCode, LPARAM flags)
{
	switch (keyCode)
	{
	case VK_UP:
		if (m_layoutText)
		{
			m_layoutText->ScrollLineUp();
			RefreshWindow();
		}
		break;

	case VK_DOWN:
		if (m_layoutText)
		{
			m_layoutText->ScrollLineDown();
			RefreshWindow();
		}
		break;
	}
	return 0;
}

LRESULT TextView::OnPaint()
{
	RECT rc;
	::GetClientRect(m_owner, &rc);

	PAINTSTRUCT ps;
	BeginPaint(m_owner, &ps);
	OnPaint(ps.hdc);
	EndPaint(m_owner, &ps);
	return 0;
}

void TextView::RefreshWindow() const
{
	InvalidateRect(m_owner, nullptr, FALSE);
}

void TextView::UpdateView()
{
	::GetClientRect(m_owner, &m_clientRect);
	OnWindowResize(m_clientRect.right - m_clientRect.left, m_clientRect.bottom - m_clientRect.top);
}


LRESULT TextView::OnMouseActivate(HWND hwndTop, UINT nHitTest, UINT nMessage) const
{
	SetFocus(m_owner);
	return MA_ACTIVATE;
}


LRESULT TextView::WndProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_ERASEBKGND:
		return 1;

	case WM_PAINT:
		return OnPaint();

	case WM_SIZE:
		::GetClientRect(m_owner, &m_clientRect);
		OnWindowResize(LOWORD(lParam), HIWORD(lParam));
		return 0;

	case WM_KEYDOWN:
		return OnKeyDown(wParam, lParam);

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

	return DefWindowProc(m_owner, msg, wParam, lParam);
}

}