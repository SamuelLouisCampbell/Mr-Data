#include "SendTextMode.h"
#include "LineMaker.h"
#include "BoxOutline.h"
#include "BoxFill.h"



SendTextMode::SendTextMode(Graphics& gfx)
	:
	smallText(gfx, 1.0f, 0.0f, L"assets/arial_32.spritefont"),
	largeText(gfx, 1.0f, 0.0f, L"assets/arial_64.spritefont"),
	sysText(gfx, 1.0f, 0.0f, L"assets/consolas_16.spritefont"),
	tc(gfx, { 20.0f, 20.0f, 1.0f }, 20.0f, Colors::White)

{
	sysText.Bind(gfx);

	pos_1 = { 200.0f,  100.0f, 1.0f };
	pos_2 = { gfx.GetWindowWidth() - 200.0f, (gfx.GetWindowHeight() / 2.0f) - 50.0f, 1.0f };
	pos_3 = { 200.0f, (gfx.GetWindowHeight() / 2.0f) + 50.0f, 1.0f };
	pos_4 = { gfx.GetWindowWidth() - 200.0f, gfx.GetWindowHeight() - 100.0f, 1.0f };

	borders.emplace_back(std::make_unique<BoxOutline>(gfx, pos_1, pos_2, Colors::Grey));
	borders.emplace_back(std::make_unique<BoxFill>(gfx, pos_1, pos_2, Colors::Black));
	borders.emplace_back(std::make_unique<BoxOutline>(gfx, pos_3, pos_4, Colors::Grey));
	borders.emplace_back(std::make_unique<BoxFill>(gfx, pos_3, pos_4, Colors::Black));
}

bool SendTextMode::SetCursorCollision(const std::pair<int, int>& mp, const DirectX::XMFLOAT3& pos_1,
	const DirectX::XMFLOAT3& pos_2, ImGuiMouseCursor IMGui_Cursor)
{
	if (mp.first >= pos_1.x && mp.first < pos_2.x &&
		mp.second >= pos_1.y && mp.second < pos_2.y)
	{
		ImGui::SetMouseCursor(IMGui_Cursor);
		return true;
	}
	return false;
}
  
void SendTextMode::Update(Window& wnd)
{
	auto mp = wnd.mouse.GetPos();
	bool txtArea0 = SetCursorCollision(mp, pos_1, pos_2, ImGuiMouseCursor_TextInput);
	bool txtArea1 = SetCursorCollision(mp, pos_3, pos_4, ImGuiMouseCursor_TextInput);

	if ((wnd.mouse.LeftIsPressed() && txtArea0) || focusArea0)
	{
		tc.UpdatePos(wnd.Gfx(), { pos_1.x + 10.0f, pos_1.y + 10.0f, 1.0f });
		tc.UpdateBlink(time.Peek());
		focusArea0 = true;
		focusArea1 = false;
	}
	if ((wnd.mouse.LeftIsPressed() && txtArea1) || focusArea1)
	{
		tc.UpdatePos(wnd.Gfx(), { pos_3.x + 10.0f, pos_3.y + 10.0f, 1.0f });
		tc.UpdateBlink(time.Peek());
		focusArea1 = true;
		focusArea0 = false;
	}
	if (wnd.mouse.LeftIsPressed() && !txtArea0 && !txtArea1)
	{
		focusArea0 = false;
		focusArea1 = false;
	}
	
}

void SendTextMode::Render(Graphics& gfx)
{

	//User Interface text
	{ 
		sysText.SetPos({ pos_1.x, pos_1.y });
		sysText.Draw(L"Small Text");
		sysText.SetPos({ pos_3.x, pos_3.y });
		sysText.Draw(L"Large Text");
	} 
	for (auto& line : borders)
	{
		line->Draw(gfx);
	}
	if (focusArea0 || focusArea1)
	{
		tc.Draw(gfx);
	}

}
