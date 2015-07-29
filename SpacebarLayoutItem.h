#ifndef _9A12C8EA_6D5A_40D9_A7D2_A68C3BE45F6B_
#define _9A12C8EA_6D5A_40D9_A7D2_A68C3BE45F6B_

#include "AbstractLayoutItem.h"
#include <string>

namespace kofax
{

struct AbstractStackLayout;

class SpacebarLayoutItem 
	: public AbstractLayoutItem
{
public:
	static AbstractLayoutItem* MakeSpaces(const AbstractStackLayout& layout,
		const wchar_t* begin, const wchar_t* end, bool newLine,
		const std::shared_ptr<IStyleView>& style);
private:
	SpacebarLayoutItem(const IDrawableElement& layout, size_t spaceCount, bool endOfLine);

	void SetTop(int x, int y) final override;
	void SetDisplayText(const wchar_t* begin, const wchar_t* end) final override;

	void OnPaint(HDC hdc) final override;
	void OnWindowResize(int width, int height) final override;

private:
	const std::wstring m_text;
	long m_lineHeight;
	bool m_EOL;
};

}

#endif
