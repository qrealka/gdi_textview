#ifndef _25A92113_39DD_4ACC_B724_A600DA404880_
#define _25A92113_39DD_4ACC_B724_A600DA404880_

#include "IDrawableElement.h"

namespace kofax
{

struct IListModel;
struct IStackLayoutView;

struct IListView : public IDrawableElement
{
	virtual LRESULT WndProc(UINT msg, WPARAM wParam, LPARAM lParam) = 0;
	virtual void SetLayout(IStackLayoutView* const layout) = 0;
	virtual void UpdateView() = 0;

protected:
	IListView() = default;

private:
	IListView(const IListView&) = delete;
	IListView& operator=(const IListView&) = delete;
};

}

#endif
