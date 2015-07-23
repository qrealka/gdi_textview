#include "UnicodeFile.h"
#include "EncodingDetect.h"
#include <locale>
#include <iostream>
#include <codecvt>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cstdint>
#include <ios>
#include <assert.h>

namespace
{

std::locale DetectLocale(const char* begin, const char* end, const std::locale& defaultLocale)
{
	using kofax::TextEncoding;

	static const char UTF_8_BOM[] = "\xEF\xBB\xBF";
	static const char UTF_16_LE_BOM[] = "\xFF\xFE";
	static const char UTF_16_BE_BOM[] = "\xFE\xFF";

	assert(begin != nullptr);
	assert(end != nullptr);
	assert(begin <= end);

	const size_t size = std::distance(begin, end);

	if (size >= 3 && std::equal(UTF_8_BOM, UTF_8_BOM + 3, begin)) // danger but use memcmp
		return std::locale(defaultLocale, new std::codecvt_utf8_utf16<wchar_t, 0x10ffff, std::consume_header>); //UnicodeType::Utf8_BOM;

	if (size >= 2) 
	{
		if (std::equal(UTF_16_LE_BOM, UTF_16_LE_BOM + 2, begin)) // danger but use memcmp
			return std::locale(defaultLocale, new std::codecvt_utf16<wchar_t, 0x10ffff, std::consume_header>); // UnicodeType::Utf16_LE_BOM;

		if (std::equal(UTF_16_BE_BOM, UTF_16_BE_BOM + 2, begin)) // danger but use memcmp
			return std::locale(defaultLocale, new std::codecvt_utf16<wchar_t, 0x10ffff, std::consume_header>); // UnicodeType::Utf16_BE_BOM;
	}

	kofax::EncodeDetector detector;

	switch (detector.Detect(begin, end))
	{
	case TextEncoding::UTF8:
		return std::locale(defaultLocale, new std::codecvt_utf8_utf16<wchar_t, 0x10ffff, std::consume_header>);;
	case TextEncoding::UTF16LE:
		return std::locale(defaultLocale, new std::codecvt_utf16<wchar_t, 0x10ffff, std::little_endian>);
	case TextEncoding::UTF16BE:
		return std::locale(defaultLocale, new std::codecvt_utf16<wchar_t>);
	case TextEncoding::UTF32LE:
	case TextEncoding::UTF32BE:
		throw new std::runtime_error("not supported UTF-32!");
	default:
		return std::locale(defaultLocale, new std::codecvt<char16_t, char, mbstate_t>);
	}

	// remove it
	return defaultLocale;
}

std::wifstream& SafeGetLine(std::wifstream& in, std::wstring& line)
{
	line.clear();
	std::wifstream::sentry se(in, true);

	auto* buf = in.rdbuf();
	for (;;)
	{
		int c = buf->sbumpc();
		switch (c) {
		case 0xfeff:
		case 0xfffe:
			continue;
		case L'\n':
			return in;
		case L'\r':
		{
			auto ch = buf->sgetc();
			if (ch == L'\n' || ch == 0xa00)
				buf->sbumpc();
			return in;
		}
		case WEOF:
			// Also handle the case when the last line has no line ending
			if (line.empty())
				in.setstate(std::ios::eofbit | std::ios::failbit | std::ios::badbit);
			return in;
		default:
			line += static_cast<wchar_t>(c);
		}
	}
}

template<typename OutputIterator, size_t BytesCount = 1024>
bool ReadFirstBytes(const wchar_t* fileName, OutputIterator out)
{
	assert(fileName);

	std::ifstream inputFile(fileName, std::ios::in | std::ios_base::binary | std::ios::ate);
	if (inputFile.bad())
		return false;

	size_t bufSize = min(BytesCount, inputFile.tellg());
	inputFile.seekg(0, std::ios::beg);
	if (!bufSize)
		return false;

	copy_n(std::istreambuf_iterator<char>(inputFile), bufSize, out);
	return true;
}

}

namespace kofax
{

std::shared_ptr<IStringListModel> UnicodeFile::OpenUnicodeFile(const wchar_t* fileName)
{
	std::vector<char> buf;
	buf.reserve(1024);
	if (!ReadFirstBytes(fileName, std::back_inserter(buf)))
		return std::shared_ptr<IStringListModel>();

	if (!buf.empty())
	{
		std::wifstream in(fileName, std::ios::binary | std::ios::ate);
		int64_t fsize = in.tellg();

		in.seekg(0, std::ios::beg);
		in.imbue(DetectLocale(buf.data(), buf.data() + buf.size(), in.getloc()));

		std::wstring line;
		for (bool first = true; fsize && in.good(); first = false)
		{
			//getline(in, line);
			SafeGetLine(in, line);

			if (in.bad() || in.fail())
			{
				if (first)
				{
					in.close();
					in.open(fileName, std::ios::binary);
					in.seekg(0, std::ios::beg);
					in.imbue(std::locale(in.getloc(), new std::codecvt<wchar_t, char, mbstate_t>));
					continue;
				}
				break;
			}
			else
			{
				
				if (in.eof())
					break;
			}
		}

	}
}

UnicodeFile::~UnicodeFile()
{
}

UnicodeFile::UnicodeFile(const wchar_t* fileName)
{
	
}

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