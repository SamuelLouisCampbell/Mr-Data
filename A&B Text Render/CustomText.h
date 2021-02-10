#pragma once
#include "BarebonesWindows.h"
#include "CustomTextRenderer.h"
#include "Graphics.h"

class CustomText
{
public:
    CustomText(HWND& hWnd, Graphics& gfx);
    ~CustomText();
    HWND GetHwnd() { return hwnd_; }
    void SetupRenderSystem();
    void Draw();

private:
    HRESULT CreateDeviceIndependentResources();
    HRESULT CreateDeviceResources();
    void DiscardDeviceResources();
    HRESULT DrawD2DContent();
    HRESULT DrawText();

private:
    HWND& hwnd_;

    // how much to scale a design that assumes 96-DPI pixels.
    float dpiScaleX_ = 100.0f;
    float dpiScaleY_ = 100.0f;

    // Direct2D
    ID2D1Factory* pD2DFactory_ = nullptr;
    ID2D1RenderTarget* pRT_ = nullptr;
    ID2D1SolidColorBrush* pBlackBrush_ = nullptr;
    ID2D1SolidColorBrush* pFillBrush_ = nullptr;

    // DirectWrite
    IDWriteFactory* pDWriteFactory_ = nullptr;
    IDWriteTextFormat* pTextFormat_ = nullptr;
    IDWriteTextLayout* pTextLayout_ = nullptr;
    IDWriteTextRenderer* pTextRenderer_ = nullptr;

    //My things
    IDXGISurface* pBackBuffer_ = nullptr;
    Graphics& gfx;

    const wchar_t* wszText_;
    UINT32 cTextLength_;
};

