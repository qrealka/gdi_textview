#ifndef _DB1BABB9_A054_404D_85AF_018C1B51F2CD_
#define _DB1BABB9_A054_404D_85AF_018C1B51F2CD_

#include "IListIndex.h"
#include <memory>

namespace kofax
{

class UnicodeFile;

class LineIndex 
	: public IListIndex
{
public:
	LineIndex(const std::shared_ptr<const UnicodeFile>& model, size_t lineNumber);
	~LineIndex() = default;

private:
	const std::wstring& ToString() const final override;
	bool GetNext() final override;
	bool IsValid() const final override;

private:
	std::weak_ptr<const UnicodeFile> m_ownerModel;
	size_t m_lineNumber;
};

}
#endif
