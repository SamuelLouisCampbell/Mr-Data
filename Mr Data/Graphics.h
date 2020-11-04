#pragma once
#include "BarebonesWindows.h"
#include "MyException.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <wrl.h>
#include "Vertex.h"

class Graphics
{
	friend class Bindable;
public:
	class HrException : public MyException
	{
	public:
		HrException(int line, const wchar_t* file, HRESULT hr) noexcept;
		const wchar_t* wideWhat() const noexcept override;
		const wchar_t* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::wstring GetErrorString() const noexcept;
		std::wstring GetErrorDescription() const noexcept;
	private:
		HRESULT hr;
	};
	class DeviceRemovedException : public HrException
	{
		using HrException::HrException;
	public:
		DeviceRemovedException(int line, const wchar_t* file, HRESULT hr) noexcept;
		const wchar_t* GetType() const noexcept override;
	};
public:
	Graphics(HWND hWnd, int width, int height);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();
	void EndFrame();
	void ClearBuffer(float red = 0.0f, float green = 0.0f, float blue = 0.0f) noexcept;
	float NormaliseVal(float val, float max, float min);
	void DrawIndexed(UINT count) noexcept;
	DirectX::XMMATRIX GetProjection() const noexcept;
	void SetProjection(DirectX::FXMMATRIX proj) noexcept;
private:
	DirectX::XMMATRIX projection;
	int WindowWidth = 0;
	int WindowHeight = 0;

private:
	Microsoft::WRL::ComPtr<ID3D11Device>			pDevice = nullptr;
	Microsoft::WRL::ComPtr<IDXGISwapChain>			pSwapChain = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>		pContext = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	pTarget = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	pDSV = nullptr;

};

