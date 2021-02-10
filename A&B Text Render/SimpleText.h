#include "BarebonesWindows.h"
#include "Graphics.h"

class SimpleText
{
public:
	SimpleText(HWND& hWnd, Graphics& gfx, const Color color, const wchar_t* fontName);
	~SimpleText();
	void SetupRenderSystem();
	void Draw(const wchar_t* string);
	void SetFontSize(const float size);
	void SetTextColor(const Color& col);
	void SetLineSpacing(const float spacing);
private:
	HRESULT CreateDeviceIndependentResources();
	HRESULT CreateDeviceResources();
	void ReleaseRescources();
	void ReleaseColor();
	void PrepareText(const wchar_t* string);
	void DrawText();
private:
	Graphics& gfx;
	HWND& hWnd;
	RECT rc;
	IDWriteFactory* pDWriteFactory_ = nullptr;
	IDWriteTextLayout* pTextLayout_ = nullptr;
	IDWriteTextFormat* pTextFormat_ = nullptr;
	std::wstring str;
	UINT32 cTextLength_;
	ID2D1Factory* pD2DFactory_ = nullptr;
	ID2D1RenderTarget* pBackBufferTarget = nullptr;
	IDXGISurface* pBackBuffer = nullptr;
	ID2D1SolidColorBrush* pColorBrush_ = nullptr;
	float dpiScaleX_ = 100.0f;
	float dpiScaleY_ = 100.0f;
	float lineSpacing = 1.0f;
	const wchar_t* fontName;
	Color currColor;
	float currSize = 72.0f;

};

