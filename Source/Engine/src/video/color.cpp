/*
 *	color.cpp
 *	Color
 */

#include <Video/Color.hpp>

namespace Video
{
	const Color Color::White(255, 255, 255);
	const Color Color::Black(0, 0, 0);
	const Color Color::Transparent(0, 0, 0, 0);
	const Color Color::CornflowerBlue(100, 149, 237);

	Color::Color(unsigned int r, unsigned int g, unsigned int b, unsigned int a)
		: r(static_cast<float>(r) / 255.0f),
		  g(static_cast<float>(g) / 255.0f),
		  b(static_cast<float>(b) / 255.0f),
		  a(static_cast<float>(a) / 255.0f)
	{

	}

	Color::Color()
		: r(0.0f),
		  g(0.0f),
		  b(0.0f),
		  a(1.0f)
	{

	}
}
