// Point3.hs
// Used to create class that stores 3 Coordinates
// X,Y & Z

#ifndef _POINT3.H_h 
#define _POINT3.H_h 

// Namespace used to keep Types
// There are various different types used in Library
namespace Type
{
	// Ensures Point3 is okay to use with Multiple Variables of Same Type
	// Specified via Template
	// Refer to https://en.cppreference.com/w/cpp/language/class_template

	template<typename Type>
	struct Point3
	{
	public:
		Type x; // Store X Coordinate
		Type y; // Store Y Coordinate
		Type z; // Store Z Coordinate

		// Friend is to ensure that variable can be declared this way
		// To more easily understand this please refer
		//	to Operator Overloading Topic
		// Refer to https://en.cppreference.com/w/cpp/language/operators
		// Const used to ensure value does not change

		 //This basically performs subtraction
		template<typename T>
		friend inline Point3<T> operator -(const Point3<T>& left, const Point3<T>& right)
		{
			Point3<T> result;

			result.x = left.x - right.x;
			result.y = left.y - right.y;
			result.z = left.z - right.z;

			return result;
		}

		// This basically performs Addition
		template<typename T>
		friend inline Point3<T> operator +(const Point3<T>& left, const Point3<T>& right)
		{
			Point3<T> result;

			result.x = left.x + right.x;
			result.y = left.y + right.y;
			result.z = left.z + right.z;

			return result;
		}

		// This basically performs Division with another integer
		template<typename T>
		friend inline Point3<double> operator /(const Point3<T>& left, const int16_t right)
		{
			Point3<double> result;

			result.x = left.x / right;
			result.y = left.y / right;
			result.z = left.z / right;

			return result;
		}
		// This basically performs Multiplication with another integer
		template<typename T>
		friend inline Point3<T> operator *(const Point3<T>& left, const int16_t right)
		{
			Point3<T> result;

			result.x = left.x * right;
			result.y = left.y * right;
			result.z = left.z * right;

			return result;
		}
		// This basically performs Division with another integer
		inline Point3<Type>& operator /=(const int16_t right)
		{
			x /= right;
			y /= right;
			z /= right;

			return *this;
		}
	};
}//Type

#endif



