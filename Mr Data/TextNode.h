#pragma once
#include "Bindable.h"

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
		font.reset();
	}
	void Bind(Graphics& gfx) noexcept override
	{
		font = std::make_unique<DirectX::SpriteFont>(GetDevice(gfx), filename.c_str());
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
	void setScale(const float scale_in)
	{
		scale = scale_in;
	}
	void setRotation(const float rot_in)
	{
		rotation = rot_in;
	}
	void Draw(std::wstring msg)
	{
		spriteBatchFont->Begin();

		DirectX::SimpleMath::Vector2 origin = font->MeasureString(msg.c_str());
		origin.x /= 2.0f;
		origin.y /= 2.0f;

		font->DrawString(spriteBatchFont.get(), msg.c_str(),
			fontPos,{col.r, col.g, col.b, col.a}, rotation, origin, scale);
		origin.y *= 2.0f;
		font->DrawString(spriteBatchFont.get(), msg.c_str(),
			fontPos, { col.r, col.g, col.b, col.a }, rotation, origin, scale);

		spriteBatchFont->End();
	}

protected:
	Graphics& gfx;
	DirectX::SimpleMath::Vector2 fontPos = { 0.0f, 0.0f };
	float scale;
	float rotation;
	std::wstring filename;
	std::vector<std::unique_ptr<DirectX::SpriteFont>> fonts;
	std::unique_ptr<DirectX::SpriteFont> font;
	std::unique_ptr<DirectX::SpriteBatch> spriteBatchFont;
	Color col;

};