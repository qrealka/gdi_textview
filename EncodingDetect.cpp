#include "EncodingDetect.h"
#include <locale>
#include <codecvt>

namespace
{

// utility wrapper to adapt locale-bound facets for wstring/wbuffer convert
template<class Facet>
struct deletable_facet : Facet
{
	template<class ...Args>
	deletable_facet(Args&& ...args) : Facet(std::forward<Args>(args)...) {}
	~deletable_facet() {}
};

}
namespace kofax
{

EncodeDetector::EncodeDetector()
{
	CComQIPtr<IMultiLanguage> pML;
	if (pML.CoCreateInstance(CLSID_CMultiLanguage) == S_OK)
	{
		if (pML.QueryInterface(&p) != S_OK)
		{
			throw std::runtime_error("Error query interface IMultiLanguage");
		}
	}
}

TextEncoding EncodeDetector::Detect(const char* begin, const char* end)
{
	//using value_type = typename std::decay< decltype(*begin) >::type;
	//static_assert(std::is_same<char, value_type>::value, "Detect only from bytes");
	assert(begin && end);
	assert(begin <= end);

	DetectEncodingInfo codePages[MaxCodePages] = {};
	auto scores = MaxCodePages;
	auto length = end - begin;

	if (SUCCEEDED(p->DetectInputCodepage(MLDETECTCP_NONE, 0, const_cast<char*>(begin), &length, codePages, &scores)))
	{
		for (auto i = 0; i < scores; ++i)
		{
			switch (codePages[i].nCodePage)
			{
			case 20127: // ASCII
			case CP_UTF8:
				return TextEncoding::UTF8;
			case CP_UTF16_LE:
				return TextEncoding::UTF16LE;
			case CP_UTF16_BE:
				return TextEncoding::UTF16BE;
			case CP_UTF32_LE:
				return TextEncoding::UTF32LE;
			case CP_UTF32_BE:
				return TextEncoding::UTF32BE;
			}
		}
	}
	else
	{
		try
		{
			std::wstring_convert<deletable_facet<std::codecvt_utf8_utf16<char16_t>>, char16_t> converter;
			return converter.from_bytes(begin, end).empty() ? TextEncoding::Ansi : TextEncoding::UTF8;
		}
		catch (std::exception&)
		{
			return TextEncoding::Ansi;
		}
	}
	return TextEncoding::Ansi;
}


}