/*
 *	color.hpp
 *	Color
 */

#ifndef __VIDEO_COLOR_HPP
#define __VIDEO_COLOR_HPP

namespace Video
{
	class Color
	{
	public:
		static const Color White;
		static const Color Black;
		static const Color Transparent;
		static const Color CornflowerBlue;

	public:
		float r, g, b, a;

		Color(unsigned int r, unsigned int g, unsigned int b, unsigned int a = 255);
		Color();
	};
}

#endif
