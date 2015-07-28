#ifndef _B735EAC9_5236_4127_8B88_C99348285EA0_
#define _B735EAC9_5236_4127_8B88_C99348285EA0_

#include "IListModel.h"
#include <vector>
#include <memory>
#include <locale>

namespace kofax
{
	class UnicodeFile 
		: public IListModel
		, public std::enable_shared_from_this<UnicodeFile>
	{
	public:
		static std::shared_ptr<IListModel> OpenUnicodeFile(const wchar_t* fileName, std::locale& locale);

		const std::wstring& GetStringLine(size_t number) const;
		size_t GetSize() const final override; // IListModel
		~UnicodeFile();

	private:
		explicit UnicodeFile(std::wifstream& file);

		UnicodeFile(const UnicodeFile&) = delete;
		UnicodeFile& operator=(const UnicodeFile&) = delete;

		IListIndex* GetIndex(size_t number) const override final; // IListModel

	private:
		std::vector<std::wstring> m_lines;
	};
}

#endif
