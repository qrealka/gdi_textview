#ifndef _DB1BABB9_A054_404D_85AF_018C1B51F2CD_
#define _DB1BABB9_A054_404D_85AF_018C1B51F2CD_

#include "IStringIndex.h"
#include <memory>

namespace kofax
{

class LineIndex : public IStringIndex
{
public:
	LineIndex(const std::shared_ptr<const IStringListModel>& model, size_t lineNumber, const std::wstring& line);	
	~LineIndex() = default;

private:
	std::weak_ptr<const IStringListModel> GetModel() const final override;
	size_t GetLineNumber() const final override;
	const std::wstring& GetLine() const final override;
	bool IsValid() const final override;

private:
	std::weak_ptr<const IStringListModel> m_ownerModel;
	const size_t m_lineNumber;
	const std::wstring* m_line; 
};

}
#endif
