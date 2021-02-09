#pragma once
#include "BarebonesWindows.h"

	struct Color
	{
		float r = 1.0f;
		float g = 1.0f;
		float b = 1.0f;
		float a = 1.0f;
	};

	struct ColorChar
	{
		unsigned char b;
		unsigned char g;
		unsigned char r;
		unsigned char a;
	};

namespace Colors
{
	static Color Red =		{ 1.0f, 0.0f, 0.0f, 1.0f };
	static Color Green =	{ 0.0f, 1.0f, 0.0f, 1.0f };
	static Color Blue =		{ 0.0f, 0.0f, 1.0f, 1.0f };
	static Color Cyan =		{ 0.0f, 1.0f, 1.0f, 1.0f };
	static Color Magenta =	{ 1.0f, 0.0f, 1.0f, 1.0f };
	static Color Yellow =	{ 1.0f, 1.0f, 0.0f, 1.0f };
	static Color Teal =		{ 0.0f, 1.0f, 0.5f, 1.0f };
	static Color Orange =	{ 1.0f, 0.5f, 0.0f, 1.0f };
	static Color Purple =	{ 0.5f, 0.0f, 1.0f, 1.0f };
	static Color Grey =		{ 0.5f, 0.5f, 0.5f, 1.0f };
	static Color White =	{ 1.0f, 1.0f, 1.0f, 1.0f };
	static Color Black =	{ 0.0f, 0.0f, 0.0f, 1.0f };
	static Color Navy =		{ 0.0f, 0.0f, 0.075f, 1.0f };
	static Color Alpha =	{ 0.0f, 0.0f, 0.0f, 0.0f };
}
