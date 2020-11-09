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
	wnd(1280, 720, L"Mr.Data 3D Window")
{
	class Factory
	{
	public:
		Factory(Graphics& gfx)
			:
			gfx(gfx)
		{}
		std::unique_ptr<Drawable> operator()()
		{
			switch (typedist(rng))
			{
			case 0:
				return std::make_unique<Pyramid>(
					gfx, rng, adist, ddist,
					odist, rdist
					);
			case 1:
				return std::make_unique<Box>(
					gfx, rng, adist, ddist,
					odist, rdist, bdist
					);
			case 2:
				return std::make_unique<Melon>(
					gfx, rng, adist, ddist,
					odist, rdist, longdist, latdist
					);
			default:
				assert(false && "bad drawable type in factory");
				return {};
			}
		}
	private:
		Graphics& gfx;
		std::mt19937 rng{ std::random_device{}() };
		std::uniform_real_distribution<float> adist{ 0.0f,PI * 2.0f };
		std::uniform_real_distribution<float> ddist{ 0.0f,PI * 0.5f };
		std::uniform_real_distribution<float> odist{ 0.0f,PI * 0.08f };
		std::uniform_real_distribution<float> rdist{ 6.0f,20.0f };
		std::uniform_real_distribution<float> bdist{ 0.4f,3.0f };
		std::uniform_int_distribution<int> latdist{ 5,20 };
		std::uniform_int_distribution<int> longdist{ 10,40 };
		std::uniform_int_distribution<int> typedist{ 0,2 };
	};

	Factory f(wnd.Gfx());
	drawables.reserve(nDrawables);
	std::generate_n(std::back_inserter(drawables), nDrawables, f);

	wnd.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, float(wnd.GetHeight()) / float(wnd.GetWidth()), 0.2f, 50.0f));
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

	if (wnd.kbd.KeyIsPressed(VK_UP))
	{
		SpeedDivisor -= 0.05f;
	}
	if (wnd.kbd.KeyIsPressed(VK_DOWN))
	{
		SpeedDivisor += 0.05f;
	}
	wnd.Gfx().ClearBuffer(0.1f, 0.0f, 0.3f);
	auto dt = time.Mark() / SpeedDivisor;

	if (!wnd.kbd.KeyIsPressed(VK_SPACE))
	{
		for (auto& d : drawables)
		{
			d->Update(dt);
		}
	}


	
}

void App::RenderFrame()
{
	imgui_mgr.NewFrame();

	std::wostringstream oss;
	oss << "Mouse X : " << wnd.mouse.GetPosX() << " Y : " << wnd.mouse.GetPosY() << " SpeedDiv : " << SpeedDivisor;
	wnd.SetTitle(oss.str());
	
	for (auto& d : drawables)
	{
		d->Draw(wnd.Gfx());
	}
	

	static bool showDemo = true;
	if (showDemo)
	{
		ImGui::ShowDemoWindow(&showDemo);
	}
	imgui_mgr.RenderFrame();

}
