#include "Graphics.h"
#pragma comment(lib,"d3d11.lib")

Graphics::Graphics(HWND hWnd)
{
	//Swap Chain Options
	DXGI_SWAP_CHAIN_DESC SwapDesc = {};
	SwapDesc.BufferDesc.Width = 0;
	SwapDesc.BufferDesc.Height = 0; //0 means look at window and set
	SwapDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	SwapDesc.BufferDesc.RefreshRate.Numerator = 0;
	SwapDesc.BufferDesc.RefreshRate.Denominator = 0;
	SwapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	SwapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	SwapDesc.SampleDesc.Count = 1;
	SwapDesc.SampleDesc.Quality = 0;
	SwapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapDesc.BufferCount = 1;
	SwapDesc.OutputWindow = HWND(4523);
	SwapDesc.Windowed = TRUE;
	SwapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	SwapDesc.Flags = 0;

	//Create Device, Front and Back Buffers, Swap Chain and Rendering Context.
	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&SwapDesc,
		&pSwapChain,
		&pDevice,
		nullptr,
		&pContext);
	if (hr != 0);
	{
		throw Graphics::HrException(__LINE__, L"Graphics.cpp", hr);
	}

	ID3D11Resource* pBackBuffer = nullptr;
	
	if(HRESULT hr =(pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&pBackBuffer))))
	{
		throw Graphics::HrException(__LINE__, L"Graphics.cpp", hr);
	}
	if (HRESULT hr = (pDevice->CreateRenderTargetView(
		pBackBuffer,
		nullptr,
		&pTarget)))
	{
		throw Graphics::HrException(__LINE__, L"Graphics.cpp", hr);
	}
	pBackBuffer->Release();
}

Graphics::~Graphics()
{
	if (pTarget != nullptr)
	{
		pTarget->Release();
	}
	if (pContext != nullptr)
	{
		pContext->Release();
	}
	if (pSwapChain != nullptr)
	{
		pSwapChain->Release();
	}
	if (pDevice != nullptr)
	{
		pDevice->Release();
	}
}

void Graphics::EndFrame()
{
	if (HRESULT hr = pSwapChain->Present(1u, 0))
	{
		if (hr == DXGI_ERROR_DEVICE_REMOVED)
		{
			//throw DeviceRemovedException(__LINE__, L"Graphics.cpp", pContext->DeviceRemovedReason());
		}
		else
		{
			throw Graphics::HrException(__LINE__, L"Graphics.cpp", hr);
		}
	}
}

void Graphics::ClearBuffer(float red, float green, float blue) noexcept
{
	const float color[] = { red, green, blue, 1.0f };
	pContext->ClearRenderTargetView(pTarget, color);
}

Graphics::HrException::HrException(int line, const wchar_t* file, HRESULT hr) noexcept
	:
	MyException(line,file),
	hr(hr)
{
}

const wchar_t* Graphics::HrException::wideWhat() const noexcept
{
	std::wostringstream oss;
	oss << GetType() << std::endl;
	oss << "[Error Code] " << GetErrorCode() << std::endl
		<< "[Description] " << GetErrorString() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const wchar_t* Graphics::HrException::GetType() const noexcept
{
	return L"Mr.Data Graphics Exception!";
}

HRESULT Graphics::HrException::GetErrorCode() const noexcept
{
	return hr;
}

std::wstring Graphics::HrException::GetErrorString() const noexcept
{
	wchar_t* buffer;
	DWORD dw = hr;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&buffer,
		512, NULL);
	std::wstring result = buffer;
	buffer = nullptr;
	return result;
}

std::wstring Graphics::HrException::GetErrorDescription() const noexcept
{
	std::wstring result = L"Graphics Exception.";
	return result;
}

const wchar_t* Graphics::DeviceRemovedException::GetType() const noexcept
{
	return L"Mr.Data Graphics Device Removed Exception!";
}
