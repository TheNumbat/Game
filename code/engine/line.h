// Program Information ////////////////////////////////////////////////////////

// Precompiler directives /////////////////////////////////////////////////////

#pragma once

// Header files ///////////////////////////////////////////////////////////////

#include "vect.h"
#include <cmath>

// Global constant definitions  ///////////////////////////////////////////////

// Class/Struct definitions  //////////////////////////////////////////////////

template <typename T>
struct segment
{
	segment()
	{
		x1 = y1 = x2 = y2 = (T)0;
	}

	segment(T x_1, T y_1, T x_2, T y_2)
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
		return sqrt((y2 - y1) * (y2 - y1) + (x2 - x1) * (x2 - x1));
	}

	v2<T> intersect(const segment& other) const
	{
		T m1 = slope();
		T m2 = other.slope();
		T x, y;

		// parallel
		if((isinf(m1) && isinf(m2)) || m1 == m2) 
		{
			return v2<T>(other.x2 - other.x1,other.y2 - other.y1);
		}
		// This is vertical
		else if(isinf(m1))
		{
			x = x1;
			y = m2 * (x1 - other.x1) + other.y1;
		}
		// Other is vertical
		else if(isinf(m2))
		{
			x = other.x1;
			y = m1 * (other.x1 - x1) + y1;
		} 
		// Neither is vertical
		else
		{
			x = (other.y1 - y1 + (m1 * x1) - (m2 * other.x1)) / (m1 - m2);
			y = m1 * (x - x1) + y1;
		}
		
		// Have we collided?
		if(x < x1 || x > x2 || y < y1 || y > y2)
		{
			// no collision
			return v2<T>(other.x2 - other.x1,other.y2 - other.y1);
		}
		// collision
		return v2<T>(x,y);
	}

	T x1, y1, x2, y2;
};

// Free function prototypes  //////////////////////////////////////////////////

// Free function implementation  //////////////////////////////////////////////

// Class/Data Structure member implementations  ///////////////////////////////

// Terminating precompiler directives  ////////////////////////////////////////
