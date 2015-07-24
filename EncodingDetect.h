#ifndef _2609BFC9_2CCB_490F_9B56_0A4D3DA0132B_
#define _2609BFC9_2CCB_490F_9B56_0A4D3DA0132B_

#include <assert.h>
#include <atlcomcli.h>
#include <MLang.h>

#define CP_UTF16_LE 1200
#define CP_UTF16_BE 1201
#define CP_UTF32_LE 12000
#define CP_UTF32_BE 12001

namespace kofax
{

enum class TextEncoding : int
{
	Ansi = CP_ACP, UTF8 = CP_UTF8, UTF16LE = CP_UTF16_LE, UTF16BE = CP_UTF16_BE, UTF32LE = CP_UTF32_LE, UTF32BE = CP_UTF32_BE
};

class EncodeDetector
	: public CComPtr<IMultiLanguage2>
{
	static const int MaxCodePages = 10;
public:
	EncodeDetector();;

	// this function is specific to our CS needs. 
	TextEncoding Detect(const char* begin, const char* end);
};

}
#endif;