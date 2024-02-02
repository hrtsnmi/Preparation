#pragma once

#include <math.h>

namespace Math
{
	// number Pi
	const float Pi = 3.1415926535897f;
	const float Pi_2 = Pi * 2.0f;
	const float Pi_3 = Pi * 3.0f;
	const float Pi_4 = Pi * 4.0f;
	const float Pi_div_2 = Pi * 0.5f;
	const float Pi_div_4 = Pi * 0.25f;
	const float Pi_Inv = 1.0f / Pi;
	const float Pi_Inv2 = Pi_Inv * 2.0f;
	const float Pi_Inv4 = Pi_Inv * 4.0f;

	// constant to convert
	// angle to radian
	// radian to angle
	//
	const float kAngToRad = Pi / 180.0f;
	const float kRadToAng = Pi_Inv * 180.0f;

	const float kFloatEps = 1e-6f;

	// convert from angle to radian
	//
	inline float AngleToRadian( float aValue )
	{
		return aValue * kAngToRad;
	}

	// convert from radian to angle
	//
	inline float RadianToAngle( float aValue )
	{
		return aValue * kRadToAng;
	}

	// getting min from two value
	//
	template< typename Type >
	inline const Type& Min( const Type& a, const Type& b )
	{
		return a < b ? a : b;
	}

	template< typename Type >
	inline const Type MinEqual( const Type& a, const Type& b )
	{
		return a <= b ? a : b;
	}

	// getting max from to value
	//
	template< typename Type >
	inline const Type& Max( const Type& a, const Type& b )
	{
		return a > b ? a : b;
	}

	template< typename Type >
	inline const Type& MaxEqueal( const Type& a, const Type& b )
	{
		return a >= b ? a : b;
	}

	// clamping value to range[min, max]
	//
	template<typename type> 
	inline const type& Clamp( const type& value, const type& minimal, const type& maximal )
	{
		//a <= b 
		if( value <= minimal )
		{
			// value is less or equal to min
			return minimal;
		}
		// value is not less, so we need to take min of max values
		return Math::Min( value, maximal );
	}

	template<typename type>
	inline type Lerp(const type& lValue, const type& rValue, float t)
	{
		t = Clamp(t, 0.0f, 1.0f);
		return lValue + (rValue - lValue) * t;
	}
}