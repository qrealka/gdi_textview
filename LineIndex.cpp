#include "LineIndex.h"
#include "UnicodeFile.h"
#include <assert.h>

namespace kofax
{

LineIndex::LineIndex(const std::shared_ptr<const UnicodeFile>& model, size_t lineNumber)
	: m_ownerModel(model)
	, m_lineNumber(lineNumber)
{
	assert(model);
	assert(lineNumber < model->GetSize());
}

const std::wstring& LineIndex::ToString() const
{
	if (auto model = m_ownerModel.lock())
		return model->GetStringLine(m_lineNumber);

	assert(false);
	throw std::runtime_error("Access to destroyed model");
}

bool LineIndex::IsValid() const
{
	if (auto model = m_ownerModel.lock())
	{
		return m_lineNumber < model->GetSize();
	}
	return false;
}

}