#ifndef _DB6A20DC_1516_4B1A_98C5_79785DDCB68A_
#define _DB6A20DC_1516_4B1A_98C5_79785DDCB68A_

#define _WIN32_WINNT 0x400
#include <windows.h>
#include <memory>

namespace kofax
{

struct IStyleView;

struct IDrawableElement
{
public:
	virtual HWND GetOwnerWindow() const = 0;

	virtual void SetStyle(std::shared_ptr<IStyleView>style) = 0;
	virtual const std::shared_ptr<IStyleView>& GetStyle() const = 0;

	virtual void GetClientRect(RECT& rect) const = 0;

	virtual void OnPaint(HDC hdc) = 0;
	virtual void OnWindowResize(int width, int height) = 0;

	virtual ~IDrawableElement() = default;

protected:
	IDrawableElement() = default;

private:
	IDrawableElement(const IDrawableElement& other) = delete;
	IDrawableElement& operator=(const IDrawableElement& other) = delete;
};

}

#endif
