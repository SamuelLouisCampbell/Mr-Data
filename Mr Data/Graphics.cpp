#include "Graphics.h"
#include <vector>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include "Gfx_Exception_Macros.h"

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib" )

namespace wrl = Microsoft::WRL;
namespace dx = DirectX;

Graphics::Graphics(HWND hWnd, int width, int height)
	:
	WindowWidth(width),
	WindowHeight(height),
	hWnd(hWnd)
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
	SwapDesc.OutputWindow = hWnd;
	SwapDesc.Windowed = TRUE;
	SwapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	SwapDesc.Flags = 0;

	HRESULT hr;
	//Create Device, Front and Back Buffers, Swap Chain and Rendering Context.
	
	hr = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&SwapDesc,
		&pSwapChain,
		&pDevice,
		nullptr,
		&pContext);
	GFX_THROW_INFO(hr);

	wrl::ComPtr<ID3D11Resource> pBackBuffer;

	hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource),&pBackBuffer);
	GFX_THROW_INFO(hr)
	hr = pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pTarget);
	GFX_THROW_INFO(hr);

	//create depth stencil state
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	wrl::ComPtr<ID3D11DepthStencilState> pDSState;
	hr = pDevice->CreateDepthStencilState(&dsDesc, &pDSState);
	GFX_THROW_INFO(hr)

	//bind depth stencil
	pContext->OMSetDepthStencilState(pDSState.Get(), 1u);

	//depth stencil texture
	wrl::ComPtr<ID3D11Texture2D> pDepthStencil;
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1u;
	descDepth.ArraySize = 1u;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1u;
	descDepth.SampleDesc.Quality = 0u;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	hr = pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil);
	GFX_THROW_INFO(hr)

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0u;
	
	hr = pDevice->CreateDepthStencilView(pDepthStencil.Get(), &descDSV, &pDSV);
	GFX_THROW_INFO(hr);

	pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), pDSV.Get());

	// configure viewport
	D3D11_VIEWPORT vp;
	vp.Width = (float)WindowWidth;
	vp.Height = (float)WindowHeight;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	pContext->RSSetViewports(1u, &vp);

	// imgui
	ImGui_ImplDX11_Init(pDevice.Get(), pContext.Get());

}

Graphics::~Graphics()
{
	
	ImGui_ImplDX11_Shutdown();
	m_texture.Reset();
	m_spriteBatch.reset();
}

void Graphics::EndFrame()
{
	HRESULT hr;
	if (IMGuiEnabled)
	{
		ImGui::Render();;
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
	
	if (FAILED(hr = pSwapChain->Present(1u, 0)))
	{
		if (hr == DXGI_ERROR_DEVICE_REMOVED)
		{
			GFX_DEVICE_REMOVED();
		}
		GFX_THROW_INFO(hr);
	}
	
}

void Graphics::BeginFrame(Color clearColor) noexcept
{
	if (IMGuiEnabled)
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}
	const float color[] = { clearColor.r,  clearColor.g,  clearColor.b, 1.0f };
	pContext->ClearRenderTargetView(pTarget.Get(), color);
	pContext->ClearDepthStencilView(pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);

}

float Graphics::NormaliseVal(float val, float max, float min)
{
	return (val - min) / (max - min);
}

void Graphics::DrawIndexed(UINT count) noexcept
{
	assert(pContext != nullptr);
	assert(count != 0u);
	pContext->DrawIndexed(count, 0u, 0u);
}

void Graphics::Draw2DTextures() noexcept
{
	m_spriteBatch->Begin();

	m_spriteBatch->Draw(m_texture.Get(), m_screenPos, nullptr, DirectX::Colors::White, 0.0f, m_origin);

	m_spriteBatch->End();
}

DirectX::XMMATRIX Graphics::GetProjection() const noexcept
{
	return projection;
}

void Graphics::SetProjection(DirectX::FXMMATRIX proj) noexcept
{
	projection = proj;
}

auto Graphics::GetDevice() noexcept
{
	return pDevice;
}

auto Graphics::GetContext()  noexcept
{
	return pContext;
}

void Graphics::EnableIMGui() noexcept
{
	IMGuiEnabled = true;
}

void Graphics::DisableIMGui() noexcept
{
	IMGuiEnabled = false;
}

bool Graphics::IsIMGuiEnabled() const noexcept
{
	return IMGuiEnabled;
}

int Graphics::GetWindowWidth() const noexcept
{
	return WindowWidth;
}

int Graphics::GetWindowHeight() const noexcept
{
	return WindowHeight;
}

ColorChar Graphics::GetPixel(int x, int y) const
{
	//Don't try to fill another buffer with this function, 
	//too much creation and destruction makes the app hang.

	wrl::ComPtr<ID3D11Texture2D> pFrame = nullptr;
	HRESULT hr = pSwapChain->GetBuffer(0, __uuidof(pFrame), &pFrame);
	GFX_THROW_INFO(hr);

	CD3D11_TEXTURE2D_DESC pStageDesc;
	wrl::ComPtr<ID3D11Texture2D> pStage = nullptr;

	pFrame->GetDesc(&pStageDesc);
	pStageDesc.BindFlags = 0u;
	pStageDesc.Usage = D3D11_USAGE_STAGING;
	pStageDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	hr = pDevice->CreateTexture2D(&pStageDesc, nullptr, &pStage);
	GFX_THROW_INFO(hr);

	pContext->CopyResource(pStage.Get(), pFrame.Get());

	//Map that gimmie my pointer to pixels fool
	D3D11_MAPPED_SUBRESOURCE map;
	map.RowPitch = WindowWidth;
	map.DepthPitch = WindowHeight;
	hr = pContext->Map(pStage.Get(), 0u, D3D11_MAP_READ, 0u, &map);
	GFX_THROW_INFO(hr);

	ColorChar* res = reinterpret_cast<ColorChar*>(map.pData);
	ColorChar col = res[x * y + x];
	pFrame.Reset();
	pStage.Reset();
	return col;
}

uint8_t* Graphics::GetFramePtr() const
{
	//Create copying/buffer texture
	wrl::ComPtr<ID3D11Texture2D> pFrame = nullptr;
	HRESULT hr = pSwapChain->GetBuffer(0, __uuidof(pFrame), &pFrame);
	GFX_THROW_INFO(hr);

	//Create staging texture
	CD3D11_TEXTURE2D_DESC pStageDesc;
	wrl::ComPtr<ID3D11Texture2D> pStage = nullptr;

	//Copy descriptors from backbuffer texture
	pFrame->GetDesc(&pStageDesc);
	pStageDesc.BindFlags = 0u;
	pStageDesc.Usage = D3D11_USAGE_STAGING;
	pStageDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	hr = pDevice->CreateTexture2D(&pStageDesc, nullptr, &pStage);
	GFX_THROW_INFO(hr);

	//Copy into staging texture
	pContext->CopyResource(pStage.Get(), pFrame.Get());

	//Map that & gimmie my pointer to pixels fool
	D3D11_MAPPED_SUBRESOURCE map;
	map.RowPitch = WindowWidth;
	map.DepthPitch = WindowHeight;
	hr = pContext->Map(pStage.Get(), 0u, D3D11_MAP_READ, 0u, &map);
	GFX_THROW_INFO(hr);

	//Make nice for others
	uint8_t* res = reinterpret_cast<uint8_t*>(map.pData);

	return res;
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
	wchar_t* pMsgBuff = nullptr;
	DWORD nMsgLen = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPTSTR>(&pMsgBuff), 0, nullptr);
	if (nMsgLen == 0)
	{
		return L"Unidentified Error Code";
	}
	std::wstring errorString = pMsgBuff;
	LocalFree(pMsgBuff);
	return errorString;
}

std::wstring Graphics::HrException::GetErrorDescription() const noexcept
{
	std::wstring result = L"Graphics Exception.";
	return result;
}

Graphics::DeviceRemovedException::DeviceRemovedException(int line, const wchar_t* file, HRESULT hr) noexcept
	:
	HrException(line, file, hr)
{
}

const wchar_t* Graphics::DeviceRemovedException::GetType() const noexcept
{
	return L"Mr.Data Graphics Device Removed Exception!";
}


