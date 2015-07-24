#ifndef _25A92113_39DD_4ACC_B724_A600DA404880_
#define _25A92113_39DD_4ACC_B724_A600DA404880_

#define _WIN32_WINNT 0x400
#include <windows.h>
#include <memory>

namespace kofax
{

struct IListModel;

struct IListView
{
	virtual LRESULT WndProc(UINT msg, WPARAM wParam, LPARAM lParam) = 0;
	virtual void SetModel(const std::shared_ptr<IListModel>& model) = 0;
	virtual ~IListView() = default;

protected:
	IListView() = default;

private:
	IListView(const IListView&) = delete;
	IListView& operator=(const IListView&) = delete;
};

}

#endif
