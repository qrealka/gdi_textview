#ifndef _9A12C8EA_6D5A_40D9_A7D2_A68C3BE45F6B_
#define _9A12C8EA_6D5A_40D9_A7D2_A68C3BE45F6B_

#include "IStackLayoutView.h"
#include "ILayoutItem.h"
#include "AbstractElement.h"

namespace kofax
{

class SpacebarLayoutItem 
	: public AbstractElement
	, public AbstractLayoutItem
{
public:
	SpacebarLayoutItem();

private:
	void SetTop(int x, int y) final override;
	void SetDisplayText(const wchar_t* begin, const wchar_t* end) final override;

	void GetClientRect(RECT& rect) const final override;
	void OnPaint(HDC hdc) final override;
	void OnWindowResize(int width, int height) final override;
};

}

#endif
