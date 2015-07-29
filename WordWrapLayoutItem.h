#ifndef _313F9085_0C6C_46C1_8EB1_F31A4D725310_
#define _313F9085_0C6C_46C1_8EB1_F31A4D725310_

#include "AbstractLayoutItem.h"

namespace kofax
{

struct AbstractStackLayout;
struct IDrawableElement;

class WordWrapLayoutItem : public AbstractLayoutItem
{
public:
	static AbstractLayoutItem* MakeWordWrappedText(const AbstractStackLayout& layout,
		const wchar_t* begin, const wchar_t* end, bool newLine,
		const std::shared_ptr<IStyleView>& style);

private:
	WordWrapLayoutItem(const IDrawableElement& layout, bool endOfLine);
	void Resize();

	// ILayoutItem
	void SetTop(int x, int y) final override;
	void SetDisplayText(const wchar_t* begin, const wchar_t* end) final override;

	// IDrawableElement
	void OnPaint(HDC hdc) final override;
	void OnWindowResize(int width, int height) final override;

private:
	const wchar_t* m_displayText;
	size_t m_displayTextLength;
	bool m_EOL;
};
}

#endif
