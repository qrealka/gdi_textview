#include "LineIndex.h"
#include "IStringListModel.h"
#include <assert.h>

namespace kofax
{

LineIndex::LineIndex(const std::shared_ptr<const IStringListModel>& model, size_t lineNumber, const std::wstring& line)
	: m_ownerModel(model)
	, m_lineNumber(lineNumber)
	, m_line(&line)
{
	assert(model);
	assert(lineNumber < model->GetStringsCount());
}

std::weak_ptr<const IStringListModel> LineIndex::GetModel() const
{
	return m_ownerModel;
}

size_t LineIndex::GetLineNumber() const
{
	return m_lineNumber;
}

const std::wstring& LineIndex::GetLine() const
{
	if (auto model = m_ownerModel.lock())
	{
		if (m_lineNumber < model->GetStringsCount())
			return *m_line;
	}
	assert(false);
	throw std::runtime_error("Access to destroyed model");
}

bool LineIndex::IsValid() const
{
	if (auto model = m_ownerModel.lock())
	{
		return m_lineNumber < model->GetStringsCount();
	}
	return false;
}

}