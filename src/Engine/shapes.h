#pragma once

#include "common.h"
#include "v2.h"
#include <vector>

struct seg
{
	seg()
	{
		x1 = y1 = x2 = y2 = 0;
	}

	seg(r32 x_1, r32 y_1, r32 x_2, r32 y_2)
	{
		x1 = x_1;
		x2 = x_2;
		y1 = y_1;
		y2 = y_2;
	}

	r32 slope() const
	{
		return (y2 - y1) / (x2 - x1);
	}

	r32 length() const
	{
		return sqrt(((y2 - y1) * (y2 - y1)) + ((x2 - x1) * (x2 - x1)));
	}

	bool includes(v2<r32> point) const
	{
		if (x1 < x2) {
			if (point.x < x1 || point.x > x2) return false;
		}
		else if (x2 < x1) {
			if (point.x < x2 || point.x > x1) return false;
		}
		if (y1 < y2) {
			if (point.y < y1 || point.y > y2) return false;
		}
		else if (y2 < y1) {
			if (point.y < y2 || point.y > y1) return false;
		}

		seg one(x1, y1, point.x, point.y);
		seg two(point.x, point.y, x2, y2);
		r32 m1 = one.slope(), m2 = two.slope();
		if (isinf(m1) && isinf(m2)) return true;
		if (isinf(m1) || isinf(m2) || m1 != m2) return false;
		return true;
	}

	seg move(r32 x, r32 y)
	{
		return seg(x1 + x, y1 + y, x2 + x, y2 + y);
	}

	v2<r32> vec() const
	{
		return v2<r32>(x2 - x1, y2 - y1);
	}

	v2<r32> intersect(const seg& other) const
	{
		r32 m1 = slope();
		r32 m2 = other.slope();
		r32 x, y;

		// parallel
		if ((isinf(m1) && isinf(m2)) || m1 == m2)
		{
			return v2<r32>(other.x2 - other.x1, other.y2 - other.y1);
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
		if (includes(v2<r32>(x, y)) && other.includes(v2<r32>(x, y)))
		{
			// collision
			return v2<r32>(x, y);
		}
		// no collision
		return v2<r32>(other.x2 - other.x1, other.y2 - other.y1);
	}

	r32 x1, y1, x2, y2;
};

struct shape
{
	virtual ~shape() {}
	virtual std::vector<seg> getSegs() = 0;
	virtual bool includes(v2<r32> pt) = 0;
};

template <typename T>
struct r2 : public shape
{
	r2()
	{
		x = (T)0;
		y = (T)0;
		w = (T)0;
		h = (T)0;
	}

	r2(T _x, T _y, T _w, T _h)
	{
		x = _x;
		y = _y;
		w = _w;
		h = _h;
	}

	std::vector<seg> getSegs()
	{
		std::vector<seg> result;
		result.push_back(seg(x, y, x + w, y));
		result.push_back(seg(x, y, x, y + h));
		result.push_back(seg(x, y + h, x + w, y + h));
		result.push_back(seg(x + w, y, x + w, y + h));
		return result;
	}

	bool includes(v2<T> pt)
	{
		return pt.x > x &&
			pt.x < x + w &&
			pt.y > y &&
			pt.y < y + h;
	}

	r2<s32> round()
	{
		return r2<s32>(std::round(x), std::round(y), std::round(w), std::round(h));
	}

	r2& operator=(const r2& src)
	{
		x = src.x;
		y = src.y;
		w = src.w;
		h = src.h;
		return *this;
	}

	bool operator==(const r2& comp) const
	{
		return (x == comp.x && y == comp.y &&
			w == comp.w && h == comp.h);
	}

	bool operator!=(const r2& comp) const
	{
		return !(x == comp.x && y == comp.y &&
			w == comp.w && h == comp.h);
	}

	r2 operator+(const r2& src) const
	{
		return r2(x, y, w + src.w, h + src.h);
	}

	r2 sweep(const r2& src) const
	{
		return r2(x - (src.w / 2.0f), y - (src.h / 2.0f), w + src.w, h + src.h);
	}

	r2 sweep_move(const r2& src) const
	{
		// return sweep(src) - v2<T>(src.x+(src.w/2.0f),src.y+(src.h/2.0f));
		return r2(x - src.x - src.w, y - src.y - src.h, w + src.w, h + src.h);
	}

	r2 operator+(const v2<T>& src) const
	{
		return r2(x + src.x, y + src.y, w, h);
	}

	r2 operator-(const v2<T>& src) const
	{
		return r2(x - src.x, y - src.y, w, h);
	}

	r2& operator+=(const r2& src)
	{
		w += src.w;
		h += src.h;
		return *this;
	}

	r2& operator+=(const v2<T>& src) const
	{
		x += src.x;
		y += src.y,
			return *this;
	}

	r2 operator-(const r2& src) const
	{
		return r2(x, y, w - src.w, h - src.h);
	}

	r2& operator-=(const r2& src)
	{
		w -= src.w;
		h -= src.h;
		return *this;
	}

	r2 operator*(const T& src) const
	{
		return r2(x * src, y * src, w * src, h * src);
	}

	r2& operator*=(const T& src)
	{
		x *= src;
		y *= src;
		h *= src;
		w *= src;
		return *this;
	}

	r2 operator*(const r2& src) const
	{
		return r2(x * src.x, y * src.y, w * src.w, h * src.h);
	}

	r2& operator*=(const r2& src)
	{
		x *= src.x;
		y *= src.y;
		w *= src.w;
		h *= src.h;
		return *this;
	}

	T x, y, w, h;
};
