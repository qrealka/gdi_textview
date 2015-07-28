#ifndef _25A92113_39DD_4ACC_B724_A600DA404880_
#define _25A92113_39DD_4ACC_B724_A600DA404880_

#include "AbstractElement.h"

namespace kofax
{

struct IStackLayoutView;

struct AbstractView : public AbstractElement
{
	virtual LRESULT WndProc(UINT msg, WPARAM wParam, LPARAM lParam) = 0;
	virtual void SetLayout(IStackLayoutView* const layout) = 0;
	virtual void UpdateView() = 0;

protected:
	explicit AbstractView(HWND owner);
};

}

#endif
