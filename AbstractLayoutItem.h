#ifndef _B6F6FE52_440F_4389_9509_2E2CC1FA6DCE_
#define _B6F6FE52_440F_4389_9509_2E2CC1FA6DCE_

#include "AbstractElement.h"

namespace kofax
{

struct IStyleView;

struct AbstractLayoutItem : public AbstractElement
{
	virtual void SetTop(int x, int y) = 0;
	virtual void SetDisplayText(const wchar_t* begin, const wchar_t* end) = 0;

protected:
	explicit AbstractLayoutItem(HWND owner);

};

}

#endif
