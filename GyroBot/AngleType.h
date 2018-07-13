// Angle.h
// Used to create class that stores 3 Coordinates
// X,Y & Z

#ifndef _ANGLETYPE.H_h 
#define _ANGLETYPE.H_h  
 
// Namespace used to keep Types
// There are various different types used in Library
namespace Type
{
	// Ensures Angle is okay to use with Multiple Variables of Same Type
	// Specified via Template
	// Refer to https://en.cppreference.com/w/cpp/language/class_template

	template<typename Type>
	struct Angle
	{
	public:
		Type roll; //Store Roll Angle
		Type pitch; //Store Pitch Angle
		Type yaw; // Store Yaw Angle

		// Friend is to ensure that variable can be declared this way
		// To more easily understand this please refer
		//	to Operator Overloading Topic
		// Refer to https://en.cppreference.com/w/cpp/language/operators
		// Const used to ensure value does not change

		// This performs subtraction
	template<typename T>
		friend inline Angle<T> operator -(const Angle<T>& left, const Angle<T>& right)
		{
			Angle<T> result;

			result.roll = left.roll - right.roll;
			result.pitch = left.pitch - right.pitch;
			result.yaw = left.yaw - right.yaw;

			return result;
		}
		// This performs addition
		template<typename T>
		friend inline Angle<T> operator +(const Angle<T>& left, const Angle<T>& right)
		{
			Angle<T> result;

			result.roll = left.roll + right.roll;
			result.pitch = left.pitch + right.pitch;
			result.yaw = left.yaw + right.yaw;

			return result;
		}
	};
}//Type

#endif




