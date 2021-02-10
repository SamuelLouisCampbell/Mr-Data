#include "CustomText.h"

CustomText::CustomText(HWND& hWnd, Graphics& gfx, const wchar_t* fontName)
    :
    hwnd_(hWnd),
    gfx(gfx),
    fontName(fontName)
{
    CreateDeviceIndependentResources();
}

CustomText::~CustomText()
{
    SafeRelease(&pD2DFactory_);
    SafeRelease(&pRT_);
    SafeRelease(&pBlackBrush_);
    SafeRelease(&pFillBrush_);
    SafeRelease(&pDWriteFactory_);
    SafeRelease(&pTextFormat_);
    SafeRelease(&pTextLayout_);
    SafeRelease(&pTextRenderer_);
    //SafeRelease(&pWICFactory_);
}

void CustomText::SetupRenderSystem()
{
    SafeRelease(&pRT_);
    SafeRelease(&pBackBuffer_);

    D2D1_RENDER_TARGET_PROPERTIES props =
        D2D1::RenderTargetProperties(
            D2D1_RENDER_TARGET_TYPE_DEFAULT,
            D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
            dpiScaleX_,
            dpiScaleY_
        );

    //Get Backbuffer from D3D
    HRESULT hr;
    hr = gfx.GetChain()->GetBuffer(
        0,
        IID_PPV_ARGS(&pBackBuffer_)
    );

    if (SUCCEEDED(hr))
    {
        // Create a Direct2D render target.
        hr = pD2DFactory_->CreateDxgiSurfaceRenderTarget(
            pBackBuffer_,
            &props,
            &pRT_
        );
    }

    hr = CreateDeviceResources();

    if (FAILED(hr))
    {
        SafeRelease(&pRT_);
        SafeRelease(&pBackBuffer_);
    }
}

void CustomText::Draw(const wchar_t* string)
{
    HRESULT hr;
    hr = PrepareText(string);
    if (SUCCEEDED(hr))
    {
        DrawD2DContent();
    }
}

void CustomText::SetFontSize(const float size)
{
}

void CustomText::SetTextFillColor(const Color& col)
{
}

void CustomText::SetTextOutlineColor(const Color& col)
{
}

void CustomText::SetLineSpacing(const float spacing)
{
}

void CustomText::SetKerning(const float kern)
{
}

void CustomText::SetOutlineStroke(const float stroke)
{
}

HRESULT CustomText::PrepareText(const wchar_t* string)
{
    str = string;
    HRESULT hr = S_OK;

    hr = pDWriteFactory_->CreateTextLayout(
        str.c_str(),
        str.size(),
        pTextFormat_,
        gfx.GetWindowWidth(),
        gfx.GetWindowHeight(),
        &pTextLayout_);

    return hr;

}

HRESULT CustomText::CreateDeviceIndependentResources()
{
    HRESULT hr = S_OK;

    // Create Direct2D factory.
    if (SUCCEEDED(hr))
    {
        hr = D2D1CreateFactory(
            D2D1_FACTORY_TYPE_SINGLE_THREADED,
            &pD2DFactory_
        );
    }

    // Create a shared DirectWrite factory.
    if (SUCCEEDED(hr))
    {
        hr = DWriteCreateFactory(
            DWRITE_FACTORY_TYPE_SHARED,
            __uuidof(IDWriteFactory),
            reinterpret_cast<IUnknown**>(&pDWriteFactory_)
        );
    }

    // The string to display.
    wszText_ = L"HOW DOES IT FEEL TO BE BACK?";
    cTextLength_ = (UINT32)wcslen(wszText_);

    // Create a text format using Gabriola with a font size of 72.
    // This sets the default font, weight, stretch, style, and locale.

    if (SUCCEEDED(hr))
    {
        hr = pDWriteFactory_->CreateTextFormat(
            fontName,
            NULL,
            DWRITE_FONT_WEIGHT_REGULAR,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            fontSize,
            L"en-us",
            &pTextFormat_
        );
    }


    // Center align (horizontally) the text.
    if (SUCCEEDED(hr))
    {
        hr = pTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    }

    // Center align (vertically) the text.
    if (SUCCEEDED(hr))
    {
        hr = pTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    }

    return hr;
}

HRESULT CustomText::CreateDeviceResources()
{
    HRESULT hr = S_OK;

    RECT rc;
    GetClientRect(hwnd_, &rc);

    D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top
    );

    if (pRT_ != nullptr)
    {
        // Create a black brush.
        if (SUCCEEDED(hr))
        {
            hr = pRT_->CreateSolidColorBrush(
                D2D1::ColorF(
                    D2D1::ColorF::White
                ),
                &pBlackBrush_);
        }
        if (SUCCEEDED(hr))
        {
            hr = pRT_->CreateSolidColorBrush(
                D2D1::ColorF(
                    D2D1::ColorF::Black
                ),
                &pFillBrush_);
        }

        if (SUCCEEDED(hr))
        {
            // Create the text renderer
            pTextRenderer_ = new (std::nothrow) CustomTextRenderer(
                pD2DFactory_,
                pRT_,
                pBlackBrush_,
                pFillBrush_
            );
        }
    }

    return hr;
}

void CustomText::DiscardDeviceResources()
{
    SafeRelease(&pRT_);
    SafeRelease(&pBlackBrush_);
    SafeRelease(&pFillBrush_);
    SafeRelease(&pTextRenderer_);
}

HRESULT CustomText::DrawText()
{
    HRESULT hr = S_OK;

    RECT rc;

    GetClientRect(
        hwnd_,
        &rc);

    D2D1_POINT_2F origin = D2D1::Point2F(
        static_cast<FLOAT>(rc.top / dpiScaleY_),
        static_cast<FLOAT>(rc.left / dpiScaleX_)
    );

    // Draw the text layout using DirectWrite and the CustomTextRenderer class.
    hr = pTextLayout_->Draw(
        NULL,
        pTextRenderer_,  // Custom text renderer.
        origin.x,
        origin.y
    );


    return hr;
}

HRESULT CustomText::DrawD2DContent()
{
    HRESULT hr;

    hr = CreateDeviceResources();


    pRT_->BeginDraw();
    pRT_->SetTransform(D2D1::IdentityMatrix());
   // pRT_->Clear(D2D1::ColorF(D2D1::ColorF::Black));

    // Call the DrawText method of this class.
    if (SUCCEEDED(hr))
    {
        hr = DrawText();
    }
    hr = pRT_->EndDraw();
    if (FAILED(hr))
    {
        DiscardDeviceResources();
    }

    return hr;
}

