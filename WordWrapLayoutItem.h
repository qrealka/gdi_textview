#ifndef _313F9085_0C6C_46C1_8EB1_F31A4D725310_
#define _313F9085_0C6C_46C1_8EB1_F31A4D725310_

#include "WordWrapLayout.h"
#include "ILayoutItem.h"

namespace kofax
{
struct IStackLayoutView;

class WordWrapLayoutItem : public ILayoutItem
{
public:
	static ILayoutItem* MakeWordWrappedText(const WordWrapLayout& layout,
		const wchar_t* begin, const wchar_t* end, IStyleView* const style);

	WordWrapLayoutItem(const IStackLayoutView& layout, int x, int y);
	void Resize();
private:
	// ILayoutItem
	void SetDisplayText(const wchar_t* begin, const wchar_t* end) final override;

	// IDrawableElement
	HWND GetOwnerWindow() const final override;
	void SetStyle(IStyleView* const style) final override;
	const std::shared_ptr<IStyleView>& GetStyle() const final override;
	void GetClientRect(RECT& rect) const final override;
	void OnPaint(HDC hdc) final override;
	void OnWindowResize(int width, int height) final override;

private:
	const HWND m_owner;
	RECT m_ownerRect;
	RECT m_clienRect;
	std::shared_ptr<IStyleView> m_style;
	const wchar_t* m_displayText;
	size_t m_displayTextLength;
};
}

#endif
