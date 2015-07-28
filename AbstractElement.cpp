#include "AbstractElement.h"

namespace kofax
{

AbstractElement::AbstractElement(HWND owner) : m_owner(owner)
{
	::GetClientRect(owner, &m_clientRect);
}

HWND AbstractElement::GetOwnerWindow() const
{
	return m_owner;
}

void AbstractElement::SetStyle(std::shared_ptr<IStyleView> style)
{
	m_style = move(style);
}

const std::shared_ptr<IStyleView>& AbstractElement::GetStyle() const
{
	return m_style;
}

void AbstractElement::GetClientRect(RECT& rect) const
{
	rect = m_clientRect;
}

}