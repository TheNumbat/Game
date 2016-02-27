// Program Information ////////////////////////////////////////////////////////

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

#include <cmath>

// Header files ///////////////////////////////////////////////////////////////

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

template <typename T>
struct v2 
{
	v2() 
	{
		x = (T)0;
		y = (T)0;
	}
	v2(T first, T second) 
	{
		x = first;
		y = second;
	}

	union 
	{
		T x;
		T a;
	};

	union 
	{
		T y;
		T b;
	};

	T length() 
	{
		return (T)sqrt(x*x + y*y);
	}

	v2& normal() 
	{
		return v2<T>(x / length(), y / length());
	}

	void normalize() 
	{
		x /= length();
		y /= length();
		z /= length();
	}

	v2 rotated(double angle) 
	{
		return v2<T>(length() * cos(angle + angle()), length() * sin(angle + angle()));
	}

	void rotate(double angle) 
	{
		x = length() * cos(angle + angle());
		y = length() * sin(angle + angle());
	}

	double angle() 
	{
		return arctan(y/x);
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
		return !(x == comp.x && y == comp.y)
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

	operator bool() const 
	{
		return x && y;
	}
};

template <typename T>
struct v3 
{
	v3() 
	{
		x = (T)0;
		y = (T)0;
		z = (T)0;
	}
	v3(T first, T second, T third) 
	{
		x = first;
		y = second;
		z = third;
	}

	union 
	{
		T x;
		T r;
	};

	union 
	{
		T y;
		T g;
	};

	union 
	{
		T z;
		T b;
	};

	T length() 
	{
		return (T)sqrt(x*x + y*y + z*z);
	}

	v3& normal() 
	{
		return v3<T>(x / length(), y / length(), z / length());
	}

	void normalize() 
	{
		x /= length();
		y /= length();
		z /= length();
	}

	T operator*(const v3& src) const 
	{
		return (x * src.x + y * src.y + z * src.z);
	}

	v3& operator=(const v3& src) 
	{
		x = src.x;
		y = src.y;
		z = src.z;
		return *this;
	}

	bool operator==(const v3& comp) const 
	{
		return (x == src.x && y == src.y && z == src.z);
	}

	bool operator!=(const v3& comp) const 
	{
		return !(x == src.x && y == src.y && z == src.z);
	}

	v3 operator+(const v3& src) const 
	{
		return v3(x + src.x, y + src.y, z + src.z);
	}

	v3& operator+=(const v3& src) 
	{
		x += src.x;
		y += src.y;
		z += src.z;
		return *this;
	}

	v3 operator-(const v3& src) const 
	{
		return v3(x - src.x, y - src.y, z - src.z);
	}

	v3& operator-=(const v3& src) 
	{
		x -= src.x;
		y -= src.y;
		z -= src.z;
		return *this;
	}

	v3 operator*(const T& src) const 
	{
		return v3(x * src, y * src, z * src);
	}

	v3& operator*=(const T& src) 
	{
		x *= src;
		y *= src;
		z *= src;
		return *this;
	}

	template <typename S>
	v3 operator*(const S& src) const 
	{
		return v3(x * src, y * src, z * src);
	}

	template <typename S>
	v3& operator*=(const S& src) 
	{
		x *= src;
		y *= src;
		z *= src;
		return *this;
	}

	operator bool() const 
	{
		return x && y && z;
	}
};

template <typename T>
struct v4 
{
	v4() 
	{
		x = (T)0;
		y = (T)0;
		z = (T)0;
		w = (T)0;
	}
	v4(T first, T second, T third, T fourth) 
	{
		x = first;
		y = second;
		z = third;
		w = fourth;
	}

	union 
	{
		T x;
		T r;
	};

	union 
	{
		T y;
		T g;
	};

	union 
	{
		T z;
		T b;
	};

	union 
	{
		T w;
		T a;
	};

	T length() 
	{
		return (T)sqrt(x*x + y*y + z*z + w*w);
	}

	v4& normal() 
	{
		return v4<T>(x / length(), y / length(), z / length(), w / length());
	}

	void normalize() 
	{
		x /= length();
		y /= length();
		z /= length();
		w /= length();
	}

	T operator*(const v4& src) const 
	{
		return (x * src.x + y * src.y + z * src.z + w * src.w);
	}

	v4& operator=(const v4& src) 
	{
		x = src.x;
		y = src.y;
		z = src.z;
		w = src.w;
		return *this;
	}

	bool operator==(const v4& comp) const 
	{
		return (x == src.x && y == src.y && z == src.z && w == src.w);
	}

	bool operator!=(const v4& comp) const 
	{
		return !(x == src.x && y == src.y && z == src.z && w == src.w);
	}

	v4 operator+(const v4& src) const 
	{
		return v4(x + src.x, y + src.y, z + src.z, w + src.w);
	}

	v4& operator+=(const v4& src) 
	{
		x += src.x;
		y += src.y;
		z += src.z;
		w += src.w;
		return *this;
	}

	v4 operator-(const v4& src) const 
	{
		return v4(x - src.x, y - src.y, z - src.z, w - src.w);
	}

	v4& operator-=(const v4& src) 
	{
		x -= src.x;
		y -= src.y;
		z -= src.z;
		w -= src.w;
		return *this;	
	}

	v4 operator*(const T& src) const 
	{
		return v4(x * src, y * src, z * src, w * src);
	}

	v4& operator*=(const T& src) 
	{
		x *= src;
		y *= src;
		z *= src;
		w *= src;
		return *this;
	}

	template <typename S>
	v4 operator*(const S& src) const 
	{
		return v4(x * src, y * src, z * src, w * src);
	}

	template <typename S>
	v4& operator*=(const S& src) 
	{
		x *= src;
		y *= src;
		z *= src;
		w *= src;
		return *this;
	}

	operator bool() const 
	{
		return x && y && z && w;
	}
};

typedef v4<uint8> color;

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
