#include "Graphics.h"
#include <vector>
#include <d3dcompiler.h>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib" )


namespace wrl = Microsoft::WRL;

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
	if (hr != 0)
	{
		throw Graphics::HrException(__LINE__, L"Graphics.cpp", hr);
	}

	wrl::ComPtr<ID3D11Resource> pBackBuffer = nullptr;
	
	hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource),&pBackBuffer);
	if (hr != 0)
	{
		throw Graphics::HrException(__LINE__, L"Graphics.cpp", hr);
	}

	hr = pDevice->CreateRenderTargetView(
		pBackBuffer.Get(),
		nullptr,
		&pTarget);
	if (hr != 0)
	{
		throw Graphics::HrException(__LINE__, L"Graphics.cpp", hr);
	}
}

Graphics::~Graphics()
{
}

void Graphics::EndFrame()
{
	if (HRESULT hr = pSwapChain->Present(1u, 0))
	{
		if (hr == DXGI_ERROR_DEVICE_REMOVED)
		{
			throw DeviceRemovedException(__LINE__, L"Graphics.cpp",pDevice->GetDeviceRemovedReason());
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
	pContext->ClearRenderTargetView(pTarget.Get(), color);
}

void Graphics::DrawTestTrtiangle()
{
	namespace wrl = Microsoft::WRL;
	HRESULT hr;

	const Vertex verts[] = {
		{0.0f, 0.5f},
		{0.5f, -0.5f},
		{-0.5f, -0.5f},
	
	};


	//Create Vertex Buffer with descriptors
	wrl::ComPtr<ID3D11Buffer>pVertexBuffer;
	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = sizeof(verts);
	bd.StructureByteStride = sizeof(Vertex);

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = verts;
	sd.SysMemPitch = 0u;
	sd.SysMemSlicePitch = 0u;

	hr = pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer);
	if (hr != 0)
	{
		throw Graphics::HrException(__LINE__, L"Graphics.cpp", hr);
	}
	//Bind Vertex Buffer to pipeline.
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	pContext->IASetVertexBuffers(0u, 1u,pVertexBuffer.GetAddressOf(),&stride, & offset);

	//create vertex shader
	wrl::ComPtr<ID3D11VertexShader> pVertexShader;
	wrl::ComPtr<ID3DBlob> pBlob;
	hr = D3DReadFileToBlob(L"VertexShader.cso", &pBlob);
	if (hr != 0)
	{
		throw Graphics::HrException(__LINE__, L"Graphics.cpp", hr);
	}
	hr = pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader);
	if (hr != 0)
	{
		throw Graphics::HrException(__LINE__, L"Graphics.cpp", hr);
	}
	//bind vertex shader
	pContext->VSSetShader(pVertexShader.Get(), nullptr, 0);


	//create input layout guide for D3D11 (2D tris in this case)
	wrl::ComPtr<ID3D11InputLayout> pInputLayout;
	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"Position",0,DXGI_FORMAT_R32G32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0}
	};
	hr = pDevice->CreateInputLayout(
		ied,
		(UINT)std::size(ied),
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		&pInputLayout
	);
	if (hr != 0)
	{
		throw Graphics::HrException(__LINE__, L"Graphics.cpp", hr);
	}
	
	pContext->IASetInputLayout(pInputLayout.Get());

	//Set Primitve Topology to Triangle
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	//Create Viewport
	D3D11_VIEWPORT vp;
	vp.Width = 1280;
	vp.Height = 720;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	// Bind viewport
	pContext->RSSetViewports(1u, &vp);


	//create PIXEL shader
	wrl::ComPtr<ID3D11PixelShader> pPixelShader;
	hr = D3DReadFileToBlob(L"PixelShader.cso", &pBlob);
	if (hr != 0) //exception
	{
		throw Graphics::HrException(__LINE__, L"Graphics.cpp", hr);
	}
	hr = pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);
	if (hr != 0) //exception
	{
		throw Graphics::HrException(__LINE__, L"Graphics.cpp", hr);
	}
	//bind pixel shader
	pContext->PSSetShader(pPixelShader.Get(), nullptr, 0);

#
	//Bind Render Target
	pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), nullptr);

	pContext->Draw(sizeof(verts), 0u);
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
