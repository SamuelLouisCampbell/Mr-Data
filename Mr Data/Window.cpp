#include "Window.h"
#include <string>
#include <sstream>

const const char* Window::WindowClass::GetName() noexcept
{
	return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
	return wndClass.hInst;
}

Window::WindowClass Window::WindowClass::wndClass;

Window::WindowClass::WindowClass() noexcept
	: 
	hInst(GetModuleHandle(nullptr))
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = 0x0020;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = NULL;
	wc.cbWndExtra = NULL;
	wc.hInstance = GetInstance();
	wc.hIcon = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON1),IMAGE_ICON, 32,32,0));
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = LPWSTR(GetName());
	wc.hIconSm = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0));
	RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass()
{
	UnregisterClass(LPWSTR(wndClassName), GetInstance());
}

Window::Window(int width, int height, LPWSTR name)
	: 
	width(width),
	height(height)
{
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	if ((AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZE | WS_SYSMENU, FALSE)) == 0)
	{
		throw MD_WND_LAST_EXCEPT();
	}
	//create window and get handle
	hWnd = CreateWindow(
		LPWSTR(WindowClass::GetName()), name,
		WS_CAPTION | WS_MINIMIZE | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr, WindowClass::GetInstance(), this
	);
	if (hWnd == nullptr)
	{
		throw MD_WND_LAST_EXCEPT();
	}
	if ((ShowWindow(hWnd, SW_SHOWDEFAULT) != 0))
	{
		throw MD_WND_LAST_EXCEPT();
	}
}


Window::~Window()
{
	DestroyWindow(hWnd);
}

void Window::SetTitle(const std::string title)
{
	if (SetWindowTextA(hWnd, title.c_str()) == 0)
	{
		throw MD_WND_LAST_EXCEPT();
	}
}

LRESULT Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	if (msg == WM_NCCREATE)
	{
		//extract window ptr to window class from creation data
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);

		//set WinAPI managed user data to store ptr to window class
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		//set message proceadure to normal handler now setup is finished
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);

	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{

	//retrieve ptr to window class
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	//foward message to window class handler
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	case WM_KILLFOCUS: //Clear keystates on loss of window focus to prevent 'zombie' states. 
		kbd.ClearState();
		break;

		// keyboard input //
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		if (!(lParam & 0x40000000) || kbd.AutoRepeatEnabled()) // filter autorepeat messages.
		{
			kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
		}
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		kbd.OnKeyReleased(static_cast<unsigned char>(wParam));
		break;
	case WM_CHAR:
		kbd.OnChar(static_cast<unsigned char>(wParam));
		break;

		// mouse messages //

		//moving
	case WM_MOUSEMOVE:
	{
		const POINTS pt = MAKEPOINTS(lParam);

		//check if inside client region
		if (pt.x >= 0 && pt.x < width &&
			pt.y >= 0 && pt.y < height)
		{
			mouse.OnMouseMove(pt.x, pt.y);
			if (!mouse.IsInWindow())
			{
				SetCapture(hWnd);
				mouse.OnMouseEnter();
			}
		}
		else
		{
			if (wParam & (MK_LBUTTON | MK_MBUTTON | MK_RBUTTON))
			{
				mouse.OnMouseMove(pt.x, pt.y);
			}
			else
			{
				ReleaseCapture();
				mouse.OnMouseLeave();
			}
		}
	}
	//left
	case WM_LBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftPressed(pt.x, pt.y);
		break;
	}
	case WM_LBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftRelease(pt.x, pt.y);
		break;
	}
	//right
	case WM_RBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightPressed(pt.x, pt.y);
		break;
	}
	case WM_RBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightRelease(pt.x, pt.y);
		break;
	}
	//center
	case WM_MBUTTONDOWN:
	{	
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnCPressed(pt.x, pt.y);
		break;
	}
	case WM_MBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnCRelease(pt.x, pt.y);
		break;
	}
	//wheel
	case WM_MOUSEWHEEL:
	{
		
		if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
		{
			const POINTS pt = MAKEPOINTS(lParam);
			mouse.OnWheelUp(pt.x, pt.y);
		}
		else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
		{
			const POINTS pt = MAKEPOINTS(lParam);
			mouse.OnWheelDown(pt.x, pt.y);
		}
		break;
	}

	default:
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}


// ERROR HANDLING BELOW...

Window::Exception::Exception(int line, const char* file, HRESULT hr)
	:
	MyException(line,file),
	hr(hr)
{}

const char* Window::Exception::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl;
	oss << "[Error Code] " << GetErrorCode() << std::endl
		<< "[Description] " << GetErrorString() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Window::Exception::GetType() const noexcept
{
	return "Mr.Data Window Exception";
}

std::string Window::Exception::TranslateErrorCode(HRESULT hr) noexcept
{
	char* pMsgBuff = nullptr;
	DWORD nMsgLen = FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&pMsgBuff), 0, nullptr);
	if (nMsgLen == 0)
	{
		return "Unidentified Error Code";
	}
	std::string errorString = pMsgBuff;
	LocalFree(pMsgBuff);
	return errorString;
}

HRESULT Window::Exception::GetErrorCode() const noexcept
{
	return hr;
}

std::string Window::Exception::GetErrorString() const noexcept
{
	return TranslateErrorCode(hr);
}
