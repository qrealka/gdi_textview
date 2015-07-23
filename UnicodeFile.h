#ifndef _B735EAC9_5236_4127_8B88_C99348285EA0_
#define _B735EAC9_5236_4127_8B88_C99348285EA0_

#include "IStringListModel.h"
#include <vector>

namespace kofax
{
	class UnicodeFile 
		: public IStringListModel
		, public std::enable_shared_from_this<IStringListModel>
	{
	public:
		static std::shared_ptr<IStringListModel> OpenUnicodeFile(const wchar_t* fileName);
		~UnicodeFile();

	private:
		explicit UnicodeFile(std::wifstream& file);

		UnicodeFile(const UnicodeFile&) = delete;
		UnicodeFile& operator=(const UnicodeFile&) = delete;

		// IStringListModel
		size_t GetStringsCount() const final override;
		std::unique_ptr<const IStringIndex> GetString(size_t number) const final override;

	private:
		std::vector<std::wstring> m_lines;
	};
}

#endif
