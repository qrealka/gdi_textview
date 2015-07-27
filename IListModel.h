#ifndef _5C029D24_395B_468E_B097_2DFEC6559325_
#define _5C029D24_395B_468E_B097_2DFEC6559325_

#include "IListIndex.h"

namespace kofax
{

struct IListIndex;

struct IListModel
{
	virtual size_t GetSize() const = 0;
	virtual const IListIndex* GetIndex(size_t number) const = 0;

	virtual ~IListModel() = default;

protected:
	IListModel() = default;

private:
	IListModel(const IListModel&) = delete;
	IListModel& operator=(const IListModel&) = delete;
};

}

#endif
