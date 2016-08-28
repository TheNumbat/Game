#pragma once

#include "common.h"
#include "v2.h"

template <typename T>
struct seg
{
	seg()
	{
		x1 = y1 = x2 = y2 = (T)0;
	}

	seg(T x_1, T y_1, T x_2, T y_2)
	{
		x1 = x_1;
		x2 = x_2;
		y1 = y_1;
		y2 = y_2;
	}

	T slope() const
	{
		return (y2 - y1) / (x2 - x1);
	}

	T length() const
	{
		return sqrt(((y2 - y1) * (y2 - y1)) + ((x2 - x1) * (x2 - x1)));
	}

	bool includes(v2<T> pos32) const
	{
		if (x1 < x2) {
			if (pos32.x < x1 || pos32.x > x2) return false;
		}
		else if (x2 < x1) {
			if (pos32.x < x2 || pos32.x > x1) return false;
		}
		if (y1 < y2) {
			if (pos32.y < y1 || pos32.y > y2) return false;
		}
		else if (y2 < y1) {
			if (pos32.y < y2 || pos32.y > y1) return false;
		}

		seg<T> one(x1, y1, pos32.x, pos32.y);
		seg<T> two(pos32.x, pos32.y, x2, y2);
		T m1 = one.slope(), m2 = two.slope();
		if (isinf(m1) && isinf(m2)) return true;
		if (isinf(m1) || isinf(m2) || m1 != m2) return false;
		return true;
	}

	seg move(T x, T y)
	{
		return seg(x1 + x, y1 + y, x2 + x, y2 + y);
	}

	v2<T> vec() const
	{
		return v2<T>(x2 - x1, y2 - y1);
	}

	v2<T> s32ersect(const seg& other) const
	{
		T m1 = slope();
		T m2 = other.slope();
		T x, y;

		// parallel
		if ((isinf(m1) && isinf(m2)) || m1 == m2)
		{
			return v2<T>(other.x2 - other.x1, other.y2 - other.y1);
		}
		// This is vertical
		else if (isinf(m1))
		{
			x = x1;
			y = (m2 * (x1 - other.x1)) + other.y1;
		}
		// Other is vertical
		else if (isinf(m2))
		{
			x = other.x1;
			y = (m1 * (other.x1 - x1)) + y1;
		}
		// Neither is vertical
		else
		{
			x = (other.y1 - y1 + (m1 * x1) - (m2 * other.x1)) / (m1 - m2);
			y = m1 * (x - x1) + y1;
		}

		// Have we collided?
		if (includes(v2<T>(x, y)) && other.includes(v2<T>(x, y)))
		{
			// collision
			return v2<T>(x, y);
		}
		// no collision
		return v2<T>(other.x2 - other.x1, other.y2 - other.y1);
	}

	T x1, y1, x2, y2;
};