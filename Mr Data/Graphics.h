#pragma once
#include "BarebonesWindows.h"
#include "MyException.h"
#include <d3d11.h>
#include <wrl.h>
#include "Vertex.h"

class Graphics
{
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
	Graphics(HWND hWnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();
	void EndFrame();
	void ClearBuffer(float red, float green, float blue) noexcept;
	void DrawTestTrtiangle();


private:
	Microsoft::WRL::ComPtr<ID3D11Device>			pDevice = nullptr;
	Microsoft::WRL::ComPtr<IDXGISwapChain>			pSwapChain = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>		pContext = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	pTarget = nullptr;

};

