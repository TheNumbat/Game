#pragma once

#include "common.h"
#include "v2.h"
#include "seg.h"
#include <vector>

template <typename T>
struct r2
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

	std::vector<seg<T>> getSegs()
	{
		std::vector<seg<T>> result;
		result.push_back(seg<T>(x, y, x + w, y));
		result.push_back(seg<T>(x, y, x, y + h));
		result.push_back(seg<T>(x, y + h, x + w, y + h));
		result.push_back(seg<T>(x + w, y, x + w, y + h));
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
		return r2<int32>(std::round(x), std::round(y), std::round(w), std::round(h));
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