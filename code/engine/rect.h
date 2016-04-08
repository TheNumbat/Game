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
		return rect2(x - (src.w / (T)2), y - (src.h / (T)2), w + src.w, h + src.h);
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

template <typename T>
struct rect3 
{
	rect3() 
	{
		x = (T)0;
		y = (T)0;
		z = (T)0;
		w = (T)0;
		h = (T)0;
		d = (T)0;
	}

	rect3(T _x, T _y, T _z, T _w, T _h, T _d) 
	{
		x = _x;
		y = _y;
		z = _z;
		w = _w;
		h = _h;
		d = _d;
	}

	rect3& operator=(const rect3& src) 
	{
		x = src.x;
		y = src.y;
		z = src.z;
		w = src.w;
		h = src.h;
		d = src.d;
	}

	bool operator==(const rect3& comp) const 
	{
		return (x == comp.x && y == comp.y &&
				w == comp.w && h == comp.h &&
				z == comp.z && d == comp.d);
	}

	bool operator!=(const rect3& comp) const 
	{
		return !(x == comp.x && y == comp.y &&
				 w == comp.w && h == comp.h &&
				 z == comp.z && d == comp.d);
	}

	rect3 operator+(const rect3& src) const 
	{
		return rect3(x, y, z, w + src.w, h + src.h, d + src.d);
	}

	rect3& operator+=(const rect3& src) 
	{
		w += src.w;
		h += src.h;
		d += src.d;
		return *this;	
	}

	rect3 operator-(const rect3& src) const 
	{
		return rect3(x, y, z, w - src.w, h - src.h, d - src.d);
	}

	rect3& operator-=(const rect3& src) 
	{
		w -= src.w;
		h -= src.h;
		d -= src.d;
		return *this;
	}

	rect3 operator*(const T& src) const 
	{
		return rect3(x * src, y * src, z * src, w * src, h * src, d * src);
	}

	rect3& operator*=(const T& src) 
	{
		x *= src;
		y *= src;
		z *= src;
		h *= src;
		w *= src;
		d *= src;
		return *this;
	}

	rect3 operator*(const rect3& src) const 
	{
		return rect3(x * src.x, y * src.y, z * src.z w * src.w, h * src.h, d * src.d);
	}

	rect3& operator*=(const rect3& src) 
	{
		x *= src.x;
		y *= src.y;
		z *= src.z;
		w *= src.w;
		h *= src.h;
		d *= src.d;
		return *this;
	}

	T x, y, z, w, h, d;
};

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
