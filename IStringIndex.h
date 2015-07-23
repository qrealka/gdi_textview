#ifndef _5EF7EAB7_D910_44F0_BEE8_8EC136D4674E_
#define _5EF7EAB7_D910_44F0_BEE8_8EC136D4674E_

#include <string>
#include <memory>

namespace kofax
{

struct IStringListModel;
struct IStringIndex
{
	virtual std::weak_ptr<const IStringListModel> GetModel() const = 0;
	virtual size_t GetLineNumber() const = 0;
	virtual const std::wstring& GetLine() const = 0;
	virtual bool IsValid() const = 0;

	virtual ~IStringIndex() = default;

protected:
	IStringIndex() = default;

private:
	IStringIndex(const IStringIndex&) = delete;
	IStringIndex& operator=(const IStringIndex&) = delete;
};

}

#endif
