#ifndef _5C029D24_395B_468E_B097_2DFEC6559325_
#define _5C029D24_395B_468E_B097_2DFEC6559325_

#include <memory>

namespace kofax
{

struct IStringLine;

struct IStringListModel
{
	virtual size_t GetStringsCount() const = 0;
	virtual std::unique_ptr<IStringLine> GetString(size_t number) = 0;
	virtual std::unique_ptr<const IStringLine> GetConstString(size_t number) const = 0;

	virtual ~IStringListModel() = default;

private:
	IStringListModel(const IStringListModel&) = delete;
	IStringListModel& operator=(const IStringListModel&) = delete;
};

}

#endif
