#pragma once
#include "BarebonesWindows.h"
#include "MyException.h"
#include "resource.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include <optional>
#include <memory>



class Window
{
public: 
	class Exception : public MyException
	{
	public:
		Exception(int line, const wchar_t* file, HRESULT hr);
		const wchar_t* wideWhat() const noexcept;
		virtual const wchar_t* GetType() const noexcept;
		static std::wstring TranslateErrorCode(HRESULT hr) noexcept;
		HRESULT GetErrorCode() const noexcept;
		std::wstring GetErrorString() const noexcept;
	private:
		HRESULT hr;
	};
	class NoGfxException : public Exception
	{
	public:
		using Exception::Exception;
		const wchar_t* GetType() const noexcept override;

	};
private:
	class WindowClass
	{
	public:
		static const wchar_t* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
		static HCURSOR GetCursor() noexcept;
	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator = (const WindowClass&) = delete;
		static constexpr const wchar_t* wndClassName = L"A&B Text Renderer";
		static WindowClass wndClass;
		HINSTANCE hInst;
		HCURSOR hCursor;
	};
public:
	Window(int width, int height, std::wstring name);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	void SetTitle(const std::wstring title);
	static std::optional<int> ProcessMessages();
	Graphics& Gfx();
	int GetWidth() const noexcept;
	int GetHeight() const noexcept;
	void DoMessageBox(const std::wstring text, const std::wstring caption);
private:
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

private:
	int width;
	int height;
	HWND hWnd;
	std::unique_ptr<Graphics> pGfx;
	
public:
	Keyboard kbd;
	Mouse mouse;



};
