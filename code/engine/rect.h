// Program Information ////////////////////////////////////////////////////////

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

// Header files ///////////////////////////////////////////////////////////////

#include "vect.h"
#include "line.h"
#include <cmath>
#include <vector>

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

template <typename T>
struct rect2 
{
	rect2() 
	{
		x = (T)0;
		y = (T)0;
		w = (T)0;
		h = (T)0;
	}

	rect2(T _x, T _y, T _w, T _h) 
	{
		x = _x;
		y = _y;
		w = _w;
		h = _h;
	}

	std::vector<segment<T>> getSegments()
	{
		std::vector<segment<T>> result;
		result.push_back(segment<T>(x,y,x+w,y));
		result.push_back(segment<T>(x,y,x,y+h));
		result.push_back(segment<T>(x,y+h,x+w,y+h));
		result.push_back(segment<T>(x+w,y,x+w,y+h));
		return result;
	}

	bool includes(v2<T> pt)
	{
		return pt.x > x &&
			   pt.x < x + w &&
			   pt.y > y && 
			   pt.y < y + h;
	}

	rect2<int32> round()
	{
		return rect2<int32>(std::round(x),std::round(y),std::round(w),std::round(h));
	}

	rect2& operator=(const rect2& src) 
	{
		x = src.x;
		y = src.y;
		w = src.w;
		h = src.h;
		return *this;
	}

	bool operator==(const rect2& comp) const 
	{
		return (x == comp.x && y == comp.y &&
				w == comp.w && h == comp.h);
	}

	bool operator!=(const rect2& comp) const 
	{
		return !(x == comp.x && y == comp.y &&
				 w == comp.w && h == comp.h);
	}

	rect2 operator+(const rect2& src) const 
	{
		return rect2(x, y, w + src.w, h + src.h);
	}

	rect2 sweep(const rect2& src) const
	{
		return rect2(x - (src.w / 2.0f), y - (src.h / 2.0f), w + src.w, h + src.h);
	}

	rect2 sweep_move(const rect2& src) const
	{
		// return sweep(src) - v2<T>(src.x+(src.w/2.0f),src.y+(src.h/2.0f));
		return rect2(x - src.x - src.w, y - src.y - src.h, w + src.w, h + src.h);
	}

	rect2 operator+(const v2<T>& src) const 
	{
		return rect2(x + src.x, y + src.y, w, h);
	}

	rect2 operator-(const v2<T>& src) const 
	{
		return rect2(x - src.x, y - src.y, w, h);
	}

	rect2& operator+=(const rect2& src) 
	{
		w += src.w;
		h += src.h;
		return *this;	
	}

	rect2& operator+=(const v2<T>& src) const 
	{
		x += src.x; 
		y += src.y,
		return *this;
	}

	rect2 operator-(const rect2& src) const 
	{
		return rect2(x, y, w - src.w, h - src.h);
	}

	rect2& operator-=(const rect2& src) 
	{
		w -= src.w;
		h -= src.h;
		return *this;
	}

	rect2 operator*(const T& src) const 
	{
		return rect2(x * src, y * src, w * src, h * src);
	}

	rect2& operator*=(const T& src) 
	{
		x *= src;
		y *= src;
		h *= src;
		w *= src;
		return *this;
	}

	rect2 operator*(const rect2& src) const 
	{
		return rect2(x * src.x, y * src.y, w * src.w, h * src.h);
	}

	rect2& operator*=(const rect2& src) 
	{
		x *= src.x;
		y *= src.y;
		w *= src.w;
		h *= src.h;
		return *this;
	}

	T x, y, w, h;
};

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
