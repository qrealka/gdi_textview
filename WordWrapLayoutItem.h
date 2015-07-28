#ifndef _313F9085_0C6C_46C1_8EB1_F31A4D725310_
#define _313F9085_0C6C_46C1_8EB1_F31A4D725310_

#include "IStackLayoutView.h"
#include "AbstractLayoutItem.h"

namespace kofax
{

class WordWrapLayoutItem : public AbstractLayoutItem
{
public:
	static AbstractLayoutItem* MakeWordWrappedText(const IStackLayoutView& layout,
		const wchar_t* begin, const wchar_t* end, bool newLine,
		const std::shared_ptr<IStyleView>& style);

private:
	WordWrapLayoutItem(const IStackLayoutView& layout, bool endOfLine);
	void Resize();

	// ILayoutItem
	void SetTop(int x, int y) final override;
	void SetDisplayText(const wchar_t* begin, const wchar_t* end) final override;

	// IDrawableElement
	void OnPaint(HDC hdc) final override;
	void OnWindowResize(int width, int height) final override;

private:
	const HWND m_owner;
	RECT m_ownerRect;
	RECT m_clienRect;
	std::shared_ptr<IStyleView> m_style;
	const wchar_t* m_displayText;
	size_t m_displayTextLength;
	bool m_EOL;
};
}

#endif
