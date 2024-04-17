#ifndef RECT_HPP
#define RECT_HPP

#include "Vec2.hpp"

#include <cmath>

template<typename T>
class Rect
{
public:
  Rect() = default;
	//Basic constructor that creates a Rect using the top left position and its dimensions
	Rect(T left, T top, T width, T height)
		:
		left(left),
		top(top),
		width(width),
    height(height)
	{}
	//Creates a Rect using its topleft corner coordinate and the dimensions of the rectangle
	Rect(Vec2<T>& topLeft, T width, T height)
		:
    Rect(topLeft.x, topLeft.y, width, height)
	{}

	//Creates a Rect using two vectors representing two opposite corners
	Rect(Vec2<T> topLeft, Vec2<T> bottomRight)
		:
		Rect(topLeft.x, bottomRight.x - topLeft.x, topLeft.y, bottomRight.y - topLeft.y)
	{}

  template<typename S>
  Rect(const Rect<S> rhs)
    :
    left((T)rhs.left),
    top((T)rhs.top),
    width((T)rhs.width),
    height((T)rhs.height)
  {}

	//Returns the position of the right side of the Rect
	T Right() const
	{
		return left + width;
	}
	//Returns the position of the bottom side of the Rect
	T Bottom() const
	{
		return top + height;
	}

	//Returns whether this Rect is overlapping with another Rect
	template<typename S>
	bool IsColliding(const Rect<S>& lhs) const
	{
		return top < (T)lhs.Bottom() && Bottom() > (T)lhs.top 
			&& left < (T)lhs.Right() && Right() > (T)lhs.left;
	}

	//Returns whether this Rect is fully contained within another Rect
	template<typename S>
	bool isWithin(const Rect<S>& lhs)
	{
		return top > (T)lhs.top && Bottom() < (T)lhs.Bottom() && Right() < (T)lhs.Right() && left > (T)lhs.left;
	}

	//Rescales the Rect by offsetting each of its sides by a set amount
	Rect<T> GetExpanded(T resize)
	{
		return Rect(top - resize, Bottom() + resize, left - resize, Right() + resize);
	}
public:
	//Values that hold the position in space for each of the Rect's sides
	T left;
	T top;
	T width;
	T height;
};

typedef Rect<int> IRect;
typedef Rect<float> FRect;
typedef Rect<double> DRect;

#endif