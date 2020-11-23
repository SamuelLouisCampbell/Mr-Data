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
	void Draw(std::wstring msg)
	{
		fonts.clear();
		fonts.emplace_back(std::make_unique<DirectX::SpriteFont>(GetDevice(gfx), filename.c_str()));
		spriteBatchFont->Begin();

		DirectX::SimpleMath::Vector2 origin = fonts[0]->MeasureString(msg.c_str());

		stringWidth = origin.x;
		stringHeight = origin.y;
		origin.x = 0.0f;
		//origin.y /= 2.0f;
		fonts[0]->DrawString(spriteBatchFont.get(), msg.c_str(),
			fontPos,{col.r, col.g, col.b, col.a}, rotation, origin, scale);

		spriteBatchFont->End();
		
	}
	void DrawCentreAlign(const std::wstring msg, const float lineSpacing)
	{
		fonts.clear();
		StringHandling sh(msg);
		const std::vector<std::wstring> strings = sh.GetStringies();
		int size = strings.size();
		DirectX::SimpleMath::Vector2 fontHeight;
		for (int i = 0; i < size; i++)
		{
			fonts.emplace_back(std::make_unique<DirectX::SpriteFont>(GetDevice(gfx), filename.c_str()));
			fontHeight = fonts[0]->MeasureString(strings[0].c_str());
		}
		spriteBatchFont->Begin();

		float totalY = 0.0f;
		if (strings.size() >= 1)
		{
			totalY = -float((strings.size() * (fontHeight.y / 2.0f )));
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


protected:
	Graphics& gfx;
	DirectX::SimpleMath::Vector2 fontPos = { 0.0f, 0.0f };
	float stringHeight;
	float stringWidth;
	float scale;
	float rotation;
	std::wstring filename;
	std::vector<std::unique_ptr<DirectX::SpriteFont>> fonts;
	std::unique_ptr<DirectX::SpriteBatch> spriteBatchFont;
	Color col;

};