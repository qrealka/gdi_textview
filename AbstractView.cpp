#include "AbstractView.h"
#include "TextViewStyle.h"

namespace kofax
{

AbstractView::AbstractView(HWND owner) : AbstractElement(owner)
{
	m_style.reset(new TextViewStyle(owner));
}

}