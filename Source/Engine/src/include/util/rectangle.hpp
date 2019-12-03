/*
 *	rectangle.hpp
 *	Rectangle
 */

#ifndef __UTIL_RECTANGLE_HPP
#define __UTIL_RECTANGLE_HPP

template <typename T>
class Rect
{
public:
	T x, y;
	T width, height;

public:
	Rect()
		: x(0),
		  y(0),
		  width(0),
		  height(0)
	{

	}

	Rect(T x, T y, T width, T height)
		: x(x),
		  y(y),
		  width(width),
		  height(height)
	{

	}

	template <typename U>
	explicit Rect(const Rect<U>& rect)
		: x(rect.x),
		  y(rect.y),
		  width(rect.width),
		  height(rect.height)
	{

	}

	bool Contains(T pX, T pY) const
	{
		return pX > x &&
			   pY > y &&
			   pX < x + width &&
			   pY < y + height;
	}

	bool Intersects(const Rect<T>& rect) const
	{
		T aX1 = x;
		T aX2 = x + width;
		T aY1 = y;
		T aY2 = y + height;
		T bX1 = rect.x;
		T bX2 = rect.x + rect.width;
		T bY1 = rect.y;
		T bY2 = rect.y + rect.height;

		return aX1 < bX2 &&
			   aX2 > bX1 &&
			   aY1 < bY2 &&
			   aY2 > bY1;
	}
};

typedef Rect<int>		IntRect;
typedef Rect<float>		FloatRect;

#endif
