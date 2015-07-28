#include "UnicodeFile.h"
#include "LineIndex.h"
#include "EncodingDetect.h"

#include <codecvt>
#include <fstream>
#include <vector>
#include <iomanip>
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

template<typename OutputIterator>
bool ReadFirstBytes(const wchar_t* fileName, size_t bytesCount, OutputIterator out)
{
	assert(fileName);

	std::ifstream inputFile(fileName, std::ios::in | std::ios_base::binary | std::ios::ate);
	if (inputFile.bad())
		return false;

	size_t bufSize = min(bytesCount, inputFile.tellg());
	inputFile.seekg(0, std::ios::beg);
	if (!bufSize)
		return false;

	copy_n(std::istreambuf_iterator<char>(inputFile), bufSize, out);
	return true;
}

}

namespace kofax
{
	
std::shared_ptr<IListModel> UnicodeFile::OpenUnicodeFile(const wchar_t* fileName, std::locale& locale)
{
	static const size_t BufferSizeForEncodingDetector = 1024;

	std::ios::sync_with_stdio(false);
	
	std::vector<char> buf;
	buf.reserve(BufferSizeForEncodingDetector);
	if (!ReadFirstBytes(fileName, BufferSizeForEncodingDetector, std::back_inserter(buf)))
		return nullptr;

	if (!buf.empty())
	{
		std::wifstream in(fileName, std::ios::binary | std::ios::ate);
		if (!in.tellg())
			return nullptr;

		in.seekg(0, std::ios::beg);
		locale = DetectLocale(buf.data(), buf.data() + buf.size(), in.getloc());
		in.imbue(locale);
		return std::shared_ptr<UnicodeFile>(new UnicodeFile(in));
	}

	return nullptr;
}

const std::wstring& UnicodeFile::GetStringLine(size_t number) const
{
	return m_lines.at(number);
}

UnicodeFile::~UnicodeFile()
{
}

UnicodeFile::UnicodeFile(std::wifstream& file)
{
	std::wstring line;
	for (bool first = true; file.good(); first = false)
	{
		SafeGetLine(file, line);

		if (file.bad() || file.fail())
		{
			// sbumpc return EOF because wrong codecvt using in specified locale
			if (first)
				throw std::runtime_error("Cannot work with ANSI files!");
			break;
		}
		m_lines.push_back(line);
	}
}

size_t UnicodeFile::GetSize() const
{
	return m_lines.size();
}

IListIndex* UnicodeFile::GetIndex(size_t number) const
{
	return number < m_lines.size() 
		? new LineIndex(shared_from_this(), number) // std::unique_ptr<const IListIndex>(std::make_unique<LineIndex const>(shared_from_this(), number))
		: nullptr;
}

}