#include "App.h"
#include <sstream>
#include "Melon.h"
#include "Pyramid.h"
#include "MD_Math.h"
#include "Box.h"
#include <algorithm>
#include "imgui.h"


App::App()
	:
	wnd(1280, 720, L"Mr.Data 3D Window"),
	txt(wnd.Gfx(),1.0f, 0.0f, L"assets/courier_new_32.spritefont")
{
	wnd.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, float(wnd.GetHeight()) / float(wnd.GetWidth()), 0.2f, 50.0f));
	centre.x = float(wnd.GetWidth()) / 2.0f;
	centre.y = float(wnd.GetHeight()) / 2.0f;
	txt.SetPos(centre);
}

int App::Setup()
{
	while (true)
	{
		if (const auto exitCode = Window::ProcessMessages())
		{
			return *exitCode;
		}

		ComposeFrame();
		RenderFrame();
		wnd.Gfx().EndFrame();
	}

}

void App::ComposeFrame()
{	
	txt.Bind(wnd.Gfx());
	wnd.Gfx().ClearBuffer(0.0f, 0.0f, 0.0f);
	txt.setRotation(0.0f);

	if (wnd.kbd.KeyIsPressed(VK_RIGHT))
	{
		//turn it red
		textCol = { 1.0f, 0.0f, 0.0f };
		txt.setColor(textCol);
	}
	if (wnd.kbd.KeyIsPressed(VK_LEFT))
	{
		//turn it white
		textCol = { 1.0f, 1.0f, 1.0f };
		txt.setColor(textCol);
	}
	if (wnd.kbd.KeyIsPressed(VK_UP))
	{
		//alpha up
		textCol.a += 0.01f;
		textCol.r += 0.01f;
		textCol.g += 0.01f;
		textCol.b += 0.01f;
		std::wstringstream ss;
		ss << textCol.a;
		wnd.SetTitle(ss.str().c_str());
		txt.setColor(textCol);
	}
	if (wnd.kbd.KeyIsPressed(VK_DOWN))
	{
		//alpha down
		textCol.a -= 0.01f;
		textCol.r -= 0.01f;
		textCol.g -= 0.01f;
		textCol.b -= 0.01f;
		std::wstringstream ss;
		ss << textCol.a;
		wnd.SetTitle(ss.str().c_str());
		txt.setColor(textCol);
	}


}

void App::RenderFrame()
{
	//wnd.Gfx().Draw2DTextures();
	txt.Draw(L"Hello you!\nI'm a little teapot\nhow am I aligned?");
	imgui_mgr.NewFrame();
	imgui_mgr.RenderFrame();

	
}
