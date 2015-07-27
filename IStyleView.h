#ifndef _F14EC127_5E5B_494A_8ECD_C610B1739E9F_
#define _F14EC127_5E5B_494A_8ECD_C610B1739E9F_

#define _WIN32_WINNT 0x400
#include <windows.h>

namespace kofax
{

struct IStyleView
{
	virtual SIZE SizeText(HDC hdc, const wchar_t* begin, const wchar_t* end) const = 0;
	virtual void PaintText(HDC hdc, int width, long height, const wchar_t* begin, const wchar_t* end) const = 0;
	virtual void PaintBackground(HDC hdc, int width, long height) const = 0;
	virtual void GetFontMetrics(HDC hdc, long& width, long& height) const = 0;

	virtual ~IStyleView() = default;

protected:
	IStyleView() = default;

private:
	IStyleView(const IStyleView&) = delete;
	IStyleView& operator=(const IStyleView&) = delete;
};

}

#endif
