#pragma once
#include "Bindable.h"
#include "StringHandling.h"

class TextNode : public Bindable
{
public:
	TextNode(Graphics& gfx_in, float scale = 1.0f, float rotation = 0.0f, std::wstring filename = L"assets/arial_64.spritefont")
		:
		gfx(gfx_in),
		scale(scale),
		rotation(rotation),
		filename(filename)
	{}
	~TextNode()
	{
		spriteBatchFont.reset();
		for(auto& font : fonts)
			{
				font.reset();
			}
	}
	void Bind(Graphics& gfx) noexcept override
	{
		spriteBatchFont = std::make_unique<DirectX::SpriteBatch>(GetContext(gfx));
	}
	void SetPos(const DirectX::SimpleMath::Vector2 posIn)
	{
		fontPos = posIn;
	}
	void setColor(const Color& col_in)
	{
		col = col_in;
	}
	void setColor(const float r, const float g, const float b, const float a )
	{
		col = { r,g,b,a };
	}
	void setScale(const float scale_in)
	{
		scale = scale_in;
	}
	void setRotation(const float rot_in)
	{
		rotation = rot_in;
	}
	float GetHeight() const
	{
		return stringHeight;
	}
	float GetWidth() const
	{
		return stringWidth;
	}
	int GetNumTextRows() const
	{
		return size;
	}
	void DrawSimple(std::wstring msg)
	{
		try
		{
			fonts.clear();
			fonts.emplace_back(std::make_unique<DirectX::SpriteFont>(GetDevice(gfx), filename.c_str()));
			spriteBatchFont->Begin();

			//defult char stops error being thrown in event of no symbol
			const wchar_t defChar = ' ';
			fonts[0]->SetDefaultCharacter(defChar);
			DirectX::SimpleMath::Vector2 origin = fonts[0]->MeasureString(msg.c_str());

			stringWidth = origin.x;
			stringHeight = origin.y;
			origin.x = 0.0f;
				
			fonts[0]->DrawString(spriteBatchFont.get(), msg.c_str(),
			fontPos, { col.r, col.g, col.b, col.a }, rotation, origin, scale);
		}
		catch (const std::exception& e)
		{
			msg.clear();
			MessageBoxA(nullptr, e.what(), "Font Issue", MB_OK | MB_ICONSTOP);
		}
		spriteBatchFont->End();
		
	}
	void Draw(std::wstring msg, const float lineSpacing)
	{
		try
		{
			fonts.clear();
			StringHandling sh(msg);
			const std::vector<std::wstring> strings = sh.GetStringies();
			size = strings.size();
			DirectX::SimpleMath::Vector2 fontDimmsW;
			DirectX::SimpleMath::Vector2 fontDimmsH;
			const wchar_t defChar = ' '; //Set defualt char.

			for (int i = 0; i < size; i++)
			{
				fonts.emplace_back(std::make_unique<DirectX::SpriteFont>(GetDevice(gfx), filename.c_str()));
				fonts[i]->SetDefaultCharacter(defChar);
				fontDimmsW = fonts[i]->MeasureString(strings[i].c_str(), false); //measure last string for width
			}
			if (size > 0)
			{
				fontDimmsH = fonts[0]->MeasureString(L"A", false); //meaurse once for height offsets.
			}
			stringWidth = fontDimmsW.x;
			stringHeight = fontDimmsH.y;
			spriteBatchFont->Begin();
			DirectX::SimpleMath::Vector2 origin = { 0.0f, 0.0f };

			for (int i = 0; i < size; i++)
			{
				fonts[i]->DrawString(spriteBatchFont.get(), strings[i].c_str(),
					fontPos, { col.r, col.g, col.b, col.a }, rotation, origin, scale);
				origin.y -= stringHeight;
			}

			spriteBatchFont->End();
		}
		catch (const std::exception& e)
		{
			MessageBoxA(nullptr, e.what(), "Font Issue", MB_OK | MB_ICONSTOP);
		}

	}
	void DrawCentreAlign(const std::wstring msg, const float lineSpacing)
	{
		try
		{
			fonts.clear();
			StringHandling sh(msg);
			const std::vector<std::wstring> strings = sh.GetStringies();
			size = strings.size();
			DirectX::SimpleMath::Vector2 fontHeight;

			const wchar_t defChar = ' ';

			for (int i = 0; i < size; i++)
			{
				fonts.emplace_back(std::make_unique<DirectX::SpriteFont>(GetDevice(gfx), filename.c_str()));
				fonts[0]->SetDefaultCharacter(defChar);
				fontHeight = fonts[0]->MeasureString(strings[0].c_str());
			}
			spriteBatchFont->Begin();

			float totalY = 0.0f;
			if (strings.size() >= 1)
			{
				totalY = -float((strings.size() * (fontHeight.y / 2.0f)));
			}

			for (int i = 0; i < size; i++)
			{
				DirectX::SimpleMath::Vector2 origin = fonts[i]->MeasureString(strings[i].c_str());
				totalY += origin.y;
				fontPos.y = (gfx.GetWindowHeight() / 2.0f) + ((totalY / 2.0f) * lineSpacing);
				origin.x /= 2.0f;
				origin.y /= 2.0f;
				origin.y += fontHeight.y / 2.0f;
				fonts[i]->DrawString(spriteBatchFont.get(), strings[i].c_str(),
					fontPos, { col.r, col.g, col.b, col.a }, rotation, origin, scale);
			}

			spriteBatchFont->End();
		}
		catch (const std::exception& e)
		{
			MessageBoxA(nullptr, e.what(), "Font Issue", MB_OK | MB_ICONSTOP);
		}
	}


protected:
	Graphics& gfx;
	DirectX::SimpleMath::Vector2 fontPos = { 0.0f, 0.0f };
	float stringHeight = 50.0f;
	float stringWidth;
	float scale;
	float rotation;
	int size;
	std::wstring filename;
	std::vector<std::unique_ptr<DirectX::SpriteFont>> fonts;
	std::unique_ptr<DirectX::SpriteBatch> spriteBatchFont;
	Color col;
	

};