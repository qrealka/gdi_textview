#ifndef _D852E863_6DBD_4482_8E04_106BE8C61F7E_
#define _D852E863_6DBD_4482_8E04_106BE8C61F7E_

namespace kofax
{

struct IListIndex;

struct IItemDelegate
{
	virtual void OnSize(int width, int height, const IListIndex& index) = 0;
	virtual ~IItemDelegate() = default;

protected:
	IItemDelegate() = default;
	
private:
	IItemDelegate(const IItemDelegate& other) = delete;
	IItemDelegate& operator=(const IItemDelegate& other) = delete;
};

}

#endif
