#ifndef _BD5FF244_3136_4E16_82B8_F55DFF4AD0AD_
#define _BD5FF244_3136_4E16_82B8_F55DFF4AD0AD_

#include "IDrawableElement.h"

namespace kofax
{

struct AbstractElement : public IDrawableElement
{
protected:
	explicit AbstractElement(HWND owner);

	HWND GetOwnerWindow() const override;
	void SetStyle(std::shared_ptr<IStyleView> style) override;
	const std::shared_ptr<IStyleView>& GetStyle() const override;
	void GetClientRect(RECT& rect) const override;

protected:
	HWND m_owner;
	std::shared_ptr<IStyleView> m_style;
	RECT m_clientRect;
};

}

#endif
