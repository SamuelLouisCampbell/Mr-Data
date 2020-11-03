#include "Graphics.h"
#include <vector>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include "Gfx_Exception_Macros.h"

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib" )

namespace wrl = Microsoft::WRL;
namespace dx = DirectX;

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
	GFX_THROW_INFO(hr);

	wrl::ComPtr<ID3D11Resource> pBackBuffer = nullptr;
	
	hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource),&pBackBuffer);
	GFX_THROW_INFO(hr)

	hr = pDevice->CreateRenderTargetView(
		pBackBuffer.Get(),
		nullptr,
		&pTarget);
	GFX_THROW_INFO(hr);

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
	descDepth.Width = 1280u;
	descDepth.Height = 720u;
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
			GFX_DEVICE_REMOVED();
		}
		else
			GFX_THROW_INFO(hr);
	}
}

void Graphics::ClearBuffer(float red, float green, float blue) noexcept
{
	const float color[] = { red, green, blue, 1.0f };
	pContext->ClearRenderTargetView(pTarget.Get(), color);
	pContext->ClearDepthStencilView(pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void Graphics::DrawTestTrtiangle(float angle, float x, float y, float z)
{
	namespace wrl = Microsoft::WRL;
	HRESULT hr;

	const Vertex verts[] = {
		{-0.5f, -0.5f, -0.5f,	1.0f, 0.0f, 0.0f},
		{ 0.5f, -0.5f, -0.5f,	0.0f, 1.0f, 0.0f},
		{-0.5f,  0.5f, -0.5f,	0.0f, 0.0f, 1.0f},
		{ 0.5f,  0.5f, -0.5f,	1.0f, 0.5f, 0.0f},
		{-0.5f, -0.5f,  0.5f,	1.0f, 0.0f, 1.0f},
		{ 0.5f, -0.5f,  0.5f,	1.0f, 1.0f, 0.0f},
		{-0.5f,  0.5f,  0.5f,	0.0f, 1.0f, 1.0f},
		{ 0.5f,  0.5f,  0.5f,	1.0f, 0.0f, 0.5f},
	};
	/*const Vertex verts[] = {
		{-0.5f, -0.5f, -0.5f},
		{ 0.5f, -0.5f, -0.5f},
		{-0.5f,  0.5f, -0.5f},
		{ 0.5f,  0.5f, -0.5f},
		{-0.5f, -0.5f,  0.5f},
		{ 0.5f, -0.5f,  0.5f},
		{-0.5f,  0.5f,  0.5f},
		{ 0.5f,  0.5f,  0.5f},
	};
	*/
	const unsigned short indices[] =
	{
		0,2,1,	2,3,1,
		1,3,5,	3,7,5,
		2,6,3,	3,6,7,
		4,5,7,	4,7,6,
		0,4,2,	2,4,6,
		0,1,4,	1,5,4,
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

	hr = pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer);
	GFX_THROW_INFO(hr);

	//Bind Vertex Buffer to pipeline.
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	pContext->IASetVertexBuffers(0u,1u,pVertexBuffer.GetAddressOf(),&stride, & offset);


	//Create Index Buffer
	wrl::ComPtr<ID3D11Buffer>pIndexBuffer;
	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.ByteWidth = sizeof(indices);
	ibd.StructureByteStride = sizeof(int);
	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = indices;
	hr = pDevice->CreateBuffer(&ibd, &isd, &pIndexBuffer);
	GFX_THROW_INFO(hr);

	//bind index buffer to pipeline
	pContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);

	//create constant buffer for transformation matrix
	struct ConstantBuffer
	{
		dx::XMMATRIX transform;
	};

	const float screenCrush =  1280.0f / 720.0f;
	const ConstantBuffer cb =
	{
		{
			dx::XMMatrixTranspose(
			dx::XMMatrixRotationZ(angle) * 
			dx::XMMatrixRotationX(angle * 0.23f) *
			dx::XMMatrixRotationX(angle * 0.4f) *
			dx::XMMatrixTranslation(x / screenCrush, -y, z + 3.0f) *
			dx::XMMatrixPerspectiveFovLH(1.0f,screenCrush, 0.5f, 30.0f))
		}
	};

	wrl::ComPtr<ID3D11Buffer> pConstantBuffer;
	D3D11_BUFFER_DESC cbd;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0u;
	cbd.ByteWidth = sizeof(cb);
	cbd.StructureByteStride = 0u;
	D3D11_SUBRESOURCE_DATA csd = {};
	csd.pSysMem = &cb;
	hr = pDevice->CreateBuffer(&cbd, &csd, &pConstantBuffer);
	if (hr != 0)
	{
		throw Graphics::HrException(__LINE__, L"Graphics.cpp", hr);
	}

	//bind constant buffers to vertex shader
	pContext->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());

	//create another constant buffer for face colors
	struct ConstantColBuff
	{
		struct 
		{
			float r; 
			float g;
			float b;
			float a;
		}face_colors;
	};
	ConstantColBuff ccb[6] =
	{
		{1.0f, 0.0f, 0.0f },
		{0.0f, 1.0f, 0.0f },
		{0.0f, 0.0f, 1.0f },
		{1.0f, 0.5f, 0.0f },
		{1.0f, 0.0f, 1.0f },
		{1.0f, 1.0f, 0.0f },
	};
	wrl::ComPtr<ID3D11Buffer> pConstantColorBuffer;
	D3D11_BUFFER_DESC ccbd;
	ccbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	ccbd.Usage = D3D11_USAGE_DYNAMIC;
	ccbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	ccbd.MiscFlags = 0u;
	ccbd.ByteWidth = sizeof(ccb);
	ccbd.StructureByteStride = 0u;
	D3D11_SUBRESOURCE_DATA ccsd = {};
	ccsd.pSysMem = &ccb;
	hr = pDevice->CreateBuffer(&ccbd, &ccsd, &pConstantColorBuffer);
	GFX_THROW_INFO(hr)

	//bind constant buffer to pixel shader
	pContext->PSSetConstantBuffers(0u, 1u, pConstantColorBuffer.GetAddressOf());


	//create PIXEL shader
	wrl::ComPtr<ID3D11PixelShader> pPixelShader;
	wrl::ComPtr<ID3DBlob> pBlob;
	hr = D3DReadFileToBlob(L"PixelShader.cso", &pBlob);
	//hr = D3DReadFileToBlob(L"PixelShaderFaces.cso", &pBlob);
	GFX_THROW_INFO(hr)

	hr = pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);
	GFX_THROW_INFO(hr)
	//bind pixel shader
	pContext->PSSetShader(pPixelShader.Get(), nullptr, 0);


	//create vertex shader
	wrl::ComPtr<ID3D11VertexShader> pVertexShader;
	hr = D3DReadFileToBlob(L"VertexShader.cso", &pBlob);
	//hr = D3DReadFileToBlob(L"VertexShaderFaces.cso", &pBlob);
	GFX_THROW_INFO(hr)
	hr = pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader);
	GFX_THROW_INFO(hr)
	//bind vertex shader
	pContext->VSSetShader(pVertexShader.Get(), nullptr, 0);


	//create input layout guide for D3D11 (2D tris in this case)
	wrl::ComPtr<ID3D11InputLayout> pInputLayout;
	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"Color",0,DXGI_FORMAT_R32G32B32_FLOAT,0,12u,D3D11_INPUT_PER_VERTEX_DATA,0}
		
	};
	hr = pDevice->CreateInputLayout(
		ied,
		(UINT)std::size(ied),
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		&pInputLayout
	);
	GFX_THROW_INFO(hr)
	
	pContext->IASetInputLayout(pInputLayout.Get());

	//Set Primitve Topology
	//pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);



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

	pContext->DrawIndexed((UINT)std::size(indices),0u,0u);
}

float Graphics::NormaliseVal(float val, float max, float min)
{
	return (val - min) / (max - min);
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


