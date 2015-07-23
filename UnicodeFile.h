#ifndef _B735EAC9_5236_4127_8B88_C99348285EA0_
#define _B735EAC9_5236_4127_8B88_C99348285EA0_

#include "IStringListModel.h"
#include <vector>
#include <utility>



namespace kofax
{
	class UnicodeFile 
		: private IStringListModel
		, private std::enable_shared_from_this<IStringListModel>
	{
	public:
		static std::shared_ptr<IStringListModel> OpenUnicodeFile(const wchar_t* fileName);
		~UnicodeFile();

	private:
		explicit UnicodeFile(const wchar_t* fileName);

		UnicodeFile(const UnicodeFile&) = delete;
		UnicodeFile& operator=(const UnicodeFile&) = delete;

		// IStringListModel
		size_t GetStringsCount() const override;
		std::unique_ptr<IStringLine> GetString(size_t number) override;
		std::unique_ptr<const IStringLine> GetConstString(size_t number) const override;

	private:
		std::vector<std::wstring> m_lines;
	};
}

#endif
