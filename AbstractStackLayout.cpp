#include "AbstractStackLayout.h"

namespace kofax
{

AbstractStackLayout::AbstractStackLayout(const IDrawableElement& owner)
	: AbstractElement(owner.GetOwnerWindow())
{
	m_style = owner.GetStyle();
	owner.GetClientRect(m_clientRect);
}

}