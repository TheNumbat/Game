#pragma once

#include "common.h"
#include <math.h>

template<typename T>
struct v2 {
	v2() {
		x = (T)0;
		y = (T)0;
	}
	v2(T _x, T _y) {
		x = _x;
		y = _y;
	}
	~v2() {}

	v2<s32> round() const
	{
		return v2<s32>(std::round(x), std::round(y));
	}

	v2<s32> floor() const
	{
		return v2<s32>(std::floor(x), std::floor(y));
	}

	v2<s32> ceil() const
	{
		return v2<s32>(std::ceil(x), std::ceil(y));
	}

	T length() const
	{
		return std::sqrt(x*x + y*y);
	}

	v2 normal() const
	{
		return v2<T>(x / length(), y / length());
	}

	v2& normalize()
	{
		x /= length();
		y /= length();
		return *this;
	}

	v2 rotated(r32 angle)
	{
		return v2<T>(length() * std::cos(angle + angle()), length() * std::sin(angle + angle()));
	}

	v2& rotate(r32 angle)
	{
		x = length() * std::cos(angle + angle());
		y = length() * std::sin(angle + angle());
		return *this;
	}

	r32 angle()
	{
		return std::atan(y / x);
	}

	v2 parallel(const v2& src) const
	{
		return src.normal() * (*this * src.normal());
	}

	T operator*(const v2& src) const
	{
		return (x * src.x + y * src.y);
	}

	v2& operator=(const v2& src)
	{
		x = src.x;
		y = src.y;
		return *this;
	}

	bool operator==(const v2& comp) const
	{
		return (x == comp.x && y == comp.y);
	}

	bool operator!=(const v2& comp) const
	{
		return !(x == comp.x && y == comp.y);
	}

	v2 operator+(const v2& src) const
	{
		return v2(x + src.x, y + src.y);
	}

	v2& operator+=(const v2& src)
	{
		x += src.x;
		y += src.y;
		return *this;
	}

	v2 operator-(const v2& src) const
	{
		return v2(x - src.x, y - src.y);
	}

	v2& operator-=(const v2& src)
	{
		x -= src.x;
		y -= src.y;
		return *this;
	}

	v2 operator*(const T& src) const
	{
		return v2(x * src, y * src);
	}

	v2& operator*=(const T& src)
	{
		x *= src;
		y *= src;
		return *this;
	}

	template <typename S>
	v2 operator*(const S& src) const
	{
		return v2(x * src, y * src);
	}

	template <typename S>
	v2& operator*=(const S& src)
	{
		x *= src;
		y *= src;
		return *this;
	}

	template <typename S>
	v2 operator/(const S& src) const
	{
		return v2(x / src, y / src);
	}

	template <typename S>
	v2& operator/=(const S& src)
	{
		x /= src;
		y /= src;
		return *this;
	}

	operator bool() const
	{
		return x || y;
	}

	union {
		T x, a;
	};
	union {
		T y, b;
	};
};
