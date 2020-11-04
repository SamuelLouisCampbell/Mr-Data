#include "App.h"
#include <sstream>

App::App()
	:
	wnd(1280, 720, L"Mr.Data 3D Window")
{
	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> adist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> ddist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> odist(0.0f, 3.1415f * 0.3f);
	std::uniform_real_distribution<float> rdist(8.0f, 20.0f);
	for (auto i = 0; i < 100; i++)
	{
		boxes.push_back(std::make_unique<Box>(
			wnd.Gfx(), rng, adist,
			ddist, odist, rdist
			));
	}
	wnd.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, float(wnd.GetHeight()) / float(wnd.GetWidth()), 0.5f, 50.0f));
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
	wnd.Gfx().ClearBuffer(0.1f, 0.0f, 0.3f);
	auto dt = time.Mark() / 3.0f;
	for (auto& b : boxes)
	{
		b->Update(dt);
		b->Draw(wnd.Gfx());
	}
	
}

void App::RenderFrame()
{
	std::wostringstream oss;
	oss << "Mouse X : " << wnd.mouse.GetPosX() << " Y : " << wnd.mouse.GetPosY();
	wnd.SetTitle(oss.str());


}
