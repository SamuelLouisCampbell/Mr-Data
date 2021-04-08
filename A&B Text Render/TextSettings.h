#pragma once
#include "Color.h"

class TextSettings 
{
public:
	Color currFillCol;
	Color oldFillCol;
	Color currOutlineCol;
	Color oldOutlineCol;
	float largeScale;
	float smallScale;
	float lineSpacing;
	float strokeWidth;
	float kerning;
	float rotaion;
	float deltaZoom;
	float deltaAlpha;
	float offsetX;
	float offsetY;
public:
	void Serialize(std::ofstream& out) const
	{
		out.write(reinterpret_cast<const char*>(&currFillCol.r), sizeof(currFillCol.r));
		out.write(reinterpret_cast<const char*>(&currFillCol.g), sizeof(currFillCol.g));
		out.write(reinterpret_cast<const char*>(&currFillCol.b), sizeof(currFillCol.b));
		//
		out.write(reinterpret_cast<const char*>(&oldFillCol.r), sizeof(oldFillCol.r));
		out.write(reinterpret_cast<const char*>(&oldFillCol.g), sizeof(oldFillCol.g));
		out.write(reinterpret_cast<const char*>(&oldFillCol.b), sizeof(oldFillCol.b));
		//
		out.write(reinterpret_cast<const char*>(&currOutlineCol.r), sizeof(currOutlineCol.r));
		out.write(reinterpret_cast<const char*>(&currOutlineCol.g), sizeof(currOutlineCol.g));
		out.write(reinterpret_cast<const char*>(&currOutlineCol.b), sizeof(currOutlineCol.b));
		//
		out.write(reinterpret_cast<const char*>(&oldOutlineCol.r), sizeof(oldOutlineCol.r));
		out.write(reinterpret_cast<const char*>(&oldOutlineCol.g), sizeof(oldOutlineCol.g));
		out.write(reinterpret_cast<const char*>(&oldOutlineCol.b), sizeof(oldOutlineCol.b));
		//
		out.write(reinterpret_cast<const char*>(&largeScale), sizeof(largeScale));
		out.write(reinterpret_cast<const char*>(&smallScale), sizeof(smallScale));
		out.write(reinterpret_cast<const char*>(&lineSpacing), sizeof(lineSpacing));
		out.write(reinterpret_cast<const char*>(&strokeWidth), sizeof(strokeWidth));
		out.write(reinterpret_cast<const char*>(&kerning), sizeof(kerning));
		out.write(reinterpret_cast<const char*>(&rotaion), sizeof(rotaion));
		out.write(reinterpret_cast<const char*>(&deltaZoom), sizeof(deltaZoom));
		out.write(reinterpret_cast<const char*>(&deltaAlpha), sizeof(deltaAlpha));
		out.write(reinterpret_cast<const char*>(&offsetX), sizeof(offsetX));
		out.write(reinterpret_cast<const char*>(&offsetY), sizeof(offsetY));

		
	}
	void DeSerialize(std::ifstream& in)
	{
		in.read(reinterpret_cast<char*>(&currFillCol.r), sizeof(currFillCol.r));
		in.read(reinterpret_cast<char*>(&currFillCol.g), sizeof(currFillCol.g));
		in.read(reinterpret_cast<char*>(&currFillCol.b), sizeof(currFillCol.b));
		//
		in.read(reinterpret_cast<char*>(&oldFillCol.r), sizeof(oldFillCol.r));
		in.read(reinterpret_cast<char*>(&oldFillCol.g), sizeof(oldFillCol.g));
		in.read(reinterpret_cast<char*>(&oldFillCol.b), sizeof(oldFillCol.b));
		//
		in.read(reinterpret_cast<char*>(&currOutlineCol.r), sizeof(currOutlineCol.r));
		in.read(reinterpret_cast<char*>(&currOutlineCol.g), sizeof(currOutlineCol.g));
		in.read(reinterpret_cast<char*>(&currOutlineCol.b), sizeof(currOutlineCol.b));
		//
		in.read(reinterpret_cast<char*>(&oldOutlineCol.r), sizeof(oldOutlineCol.r));
		in.read(reinterpret_cast<char*>(&oldOutlineCol.g), sizeof(oldOutlineCol.g));
		in.read(reinterpret_cast<char*>(&oldOutlineCol.b), sizeof(oldOutlineCol.b));
		//
		in.read(reinterpret_cast<char*>(&largeScale), sizeof(largeScale));
		in.read(reinterpret_cast<char*>(&smallScale), sizeof(smallScale));
		in.read(reinterpret_cast<char*>(&lineSpacing), sizeof(lineSpacing));
		in.read(reinterpret_cast<char*>(&strokeWidth), sizeof(strokeWidth));
		in.read(reinterpret_cast<char*>(&kerning), sizeof(kerning));
		in.read(reinterpret_cast<char*>(&rotaion), sizeof(rotaion));
		in.read(reinterpret_cast<char*>(&deltaZoom), sizeof(deltaZoom));
		in.read(reinterpret_cast<char*>(&deltaAlpha), sizeof(deltaAlpha));
		in.read(reinterpret_cast<char*>(&offsetX), sizeof(offsetX));
		in.read(reinterpret_cast<char*>(&offsetY), sizeof(offsetY));
	}
};
