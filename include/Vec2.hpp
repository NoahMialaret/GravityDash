#ifndef VEC2_HPP
#define VEC2_HPP

#include <cmath>

template<typename T>
class Vec2
{
public:
	Vec2()
		:
		x((T)0),
		y((T)0)
	{}

	Vec2(T init)
		:
		x(init),
		y(init)
	{}

	Vec2(T xInit, T yInit)
		:
		x(xInit),
		y(yInit)
	{}

	template<typename S>
	Vec2(const Vec2<S> rhs)
		:
		x((T)rhs.x),
		y((T)rhs.y)
	{}

	Vec2 operator+(const Vec2& rhs) const
	{
		return Vec2(x + rhs.x, y + rhs.y);
	}
	Vec2& operator+=(const Vec2& rhs)
	{
		return *this = *this + rhs;
	}

	Vec2 operator-(const Vec2& rhs) const
	{
		return Vec2(x - rhs.x, y - rhs.y);
	}
	Vec2& operator-=(const Vec2& rhs)
	{
		return *this = *this - rhs;
	}

	Vec2 operator*(const T rhs) const
	{
		return Vec2(x * rhs, y * rhs);
	}
	Vec2& operator*=(const T rhs)
	{
		return *this = *this * rhs;
	}

	Vec2 operator/(const T rhs) const
	{
		return Vec2(x / rhs, y / rhs);
	}
	Vec2& operator/=(const T rhs)
	{
		return *this = *this / rhs;
	}

	bool operator==(const Vec2& rhs) const
	{
		return x == rhs.x && y == rhs.y;
	}
	bool operator!=(const Vec2& rhs) const
	{
		return x != rhs.x || y != rhs.y;
	}

	float GetLengthSquared() const
	{
		return ((float)x * (float)x) + ((float)y * (float)y);
	}

	float GetLength() const
	{
		return std::sqrt(GetLengthSquared());
	}

	Vec2 GetUnitVector() const
	{
		float len = GetLength();
		if (len > 0.0f)
		{
			return *this / len;
		}
		return *this;
	}
	Vec2& MakeUnitVector()
	{
		return *this = GetUnitVector();
	}

	T Dot(const Vec2& rhs) const
	{
		return x * rhs.x + y * rhs.y;
	}

	Vec2 Turn90Clockwise() const
	{
		return Vec2<T>(-y, x);
	}
	Vec2 Turn90AntiClockwise() const
	{
		return Vec2<T>(y, -x);
	}

public:
	T x;
	T y;
};

typedef Vec2<int> iVec2;
typedef Vec2<float> fVec2;
typedef Vec2<double> dVec2;

#endif