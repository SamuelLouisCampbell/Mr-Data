#include "SimpleText.h"

SimpleText::SimpleText(HWND& hWnd, Graphics& gfx, const Color color ,const wchar_t* fontName = L"Arial")
	:
	gfx(gfx),
	hWnd(hWnd),
	currColor(color),
	fontName(fontName)
{
	CreateDeviceIndependentResources();
}

SimpleText::~SimpleText()
{
	ReleaseRescources();
}

void SimpleText::SetupRenderSystem()
{
	SafeRelease(&pBackBuffer);
	SafeRelease(&pBackBufferTarget);

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
		IID_PPV_ARGS(&pBackBuffer)
	);

	if (SUCCEEDED(hr))
	{
		// Create a Direct2D render target.
		hr = pD2DFactory_->CreateDxgiSurfaceRenderTarget(
			pBackBuffer,
			&props,
			&pBackBufferTarget
		);
	}

	hr = CreateDeviceResources();

	if (FAILED(hr))
	{
		ReleaseRescources();
	}
}

void SimpleText::Draw(const wchar_t* string)
{

	pBackBufferTarget->BeginDraw();
	//pBackBufferTarget->SetTransform(D2D1::IdentityMatrix());
		// Call the DrawText method of this class.
	DrawText(string);
		
	pBackBufferTarget->EndDraw();


}

void SimpleText::SetFontSize(const float size)
{
	currSize = size;
}

void SimpleText::DrawText(const wchar_t* string)
{
	str = string;

	GetClientRect(hWnd, &rc);
	float width = rc.right; /// dpiScaleX_;
	float height = rc.bottom;// / dpiScaleY_;

	pDWriteFactory_->CreateTextLayout(
		str.c_str(),      // The string to be laid out and formatted.
		str.size(),  // The length of the string.
		pTextFormat_,  // The text format to apply to the string (contains font information, etc).
		width,         // The width of the layout box.
		height,        // The height of the layout box.
		&pTextLayout_);  // The IDWriteTextLayout interface pointer.
	
	pTextLayout_->SetFontSize(currSize, { 0,str.size() });
	pTextLayout_->SetLineSpacing(DWRITE_LINE_SPACING_METHOD_UNIFORM, lineSpacing * currSize, 1.0f);

	pBackBufferTarget->DrawTextLayout(
		{ 0,(currSize /2.0f) },
		pTextLayout_,
		pColorBrush_

	);
}

void SimpleText::SetTextColor(const Color& col)
{
	ReleaseColor();
	pBackBufferTarget->CreateSolidColorBrush(
		D2D1::ColorF({ col.r,col.g,col.b,col.a }),
		&pColorBrush_);
	DWRITE_TEXT_RANGE textRange1 = { 0, str.size() };
	pTextLayout_->SetDrawingEffect(pColorBrush_, textRange1);
	SafeRelease(&pTextLayout_);
}

void SimpleText::SetLineSpacing(const float spacing)
{
	lineSpacing = spacing;
}

HRESULT SimpleText::CreateDeviceIndependentResources()
{
	HRESULT hr;

	hr = D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
		&pD2DFactory_
	);
	if (SUCCEEDED(hr))
	{
		hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory),
			reinterpret_cast<IUnknown**>(&pDWriteFactory_)
		);
	}

	//Font Descriptor
	if (SUCCEEDED(hr))
	{
		hr = pDWriteFactory_->CreateTextFormat(
			fontName,                // Font family name.
			NULL,                       // Font collection (NULL sets it to use the system font collection).
			DWRITE_FONT_WEIGHT_REGULAR,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			72.0f,
			L"en-us",
			&pTextFormat_
		);
	}
	
	// Center align (horizontally) the text.
	if (SUCCEEDED(hr))
	{
		hr = pTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	}

	if (SUCCEEDED(hr))
	{
		hr = pTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	}
	return hr;
}

HRESULT SimpleText::CreateDeviceResources()
{
	HRESULT hr = 0;
	//get the window size
	GetClientRect(hWnd, &rc);

	D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

	if (pBackBufferTarget != nullptr)
	{
		// Create color brush.
		if (SUCCEEDED(hr))
		{
			hr = pBackBufferTarget->CreateSolidColorBrush(
				D2D1::ColorF({currColor.r,currColor.g,currColor.b,currColor.a}),
				&pColorBrush_
			);
		}
	}
	return hr;
}

void SimpleText::ReleaseRescources()
{
	SafeRelease(&pDWriteFactory_);
	SafeRelease(&pTextLayout_);
	SafeRelease(&pTextFormat_);
	SafeRelease(&pD2DFactory_);
	SafeRelease(&pBackBufferTarget);
	SafeRelease(&pColorBrush_);
}

void SimpleText::ReleaseColor()
{
	if (pColorBrush_)
	{
		pColorBrush_->Release();
		pColorBrush_ = nullptr;
	}
}
