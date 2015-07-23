#include "UnicodeFile.h"

namespace kofax
{

std::shared_ptr<IStringListModel> UnicodeFile::OpenUnicodeFile(const wchar_t* fileName)
{
	
}

UnicodeFile::~UnicodeFile()
{
}

UnicodeFile::UnicodeFile(const wchar_t* fileName)
{}

size_t UnicodeFile::GetStringsCount() const
{
	return m_lines.size();
}

std::unique_ptr<IStringLine> UnicodeFile::GetString(size_t number)
{
	//shared_from_this();
}

std::unique_ptr<const IStringLine> UnicodeFile::GetConstString(size_t number) const
{}
}