#pragma once
#include "BarebonesWindows.h"
#include "MyException.h"
#include <SpriteBatch.h>
#include <SimpleMath.h>
#include <WICTextureLoader.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <wrl.h>
#include "Vertex.h"
#include "Color.h"
#include "ImGuiManager.h"

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
	void BeginFrame(Color clearColor) noexcept;
	float NormaliseVal(float val, float max, float min);
	void DrawIndexed(UINT count) noexcept;
	void Draw2DTextures() noexcept;
	DirectX::XMMATRIX GetProjection() const noexcept;
	void SetProjection(DirectX::FXMMATRIX proj) noexcept;
	auto GetDevice() noexcept;
	auto GetContext() noexcept;
	void EnableIMGui() noexcept;
	void DisableIMGui() noexcept;
	bool IsIMGuiEnabled() const noexcept;
	int GetWindowWidth() const noexcept;
	int GetWindowHeight() const noexcept;
	ColorChar GetPixel(int x, int y) const;
	uint8_t* GetFramePtr() const;

private:
	bool IMGuiEnabled = true;
	DirectX::XMMATRIX projection;
	int WindowWidth = 0;
	int WindowHeight = 0;

private:
	Microsoft::WRL::ComPtr<ID3D11Device>			pDevice = nullptr;
	Microsoft::WRL::ComPtr<IDXGISwapChain>			pSwapChain = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>		pContext = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	pTarget = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	pDSV = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>m_texture = nullptr;
	
	

	//image texture stuff
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	DirectX::SimpleMath::Vector2 m_screenPos;
	DirectX::SimpleMath::Vector2 m_origin;

};

