#ifndef _5EF7EAB7_D910_44F0_BEE8_8EC136D4674E_
#define _5EF7EAB7_D910_44F0_BEE8_8EC136D4674E_

#include <string>

namespace kofax
{

struct IListModel;

struct IListIndex
{
	virtual const std::wstring& ToString() const = 0;
	virtual bool GetNext() = 0;
	virtual bool IsValid() const = 0;

	virtual ~IListIndex() = default;

protected:
	IListIndex() = default;

private:
	IListIndex(const IListIndex&) = delete;
	IListIndex& operator=(const IListIndex&) = delete;
};

}

#endif
