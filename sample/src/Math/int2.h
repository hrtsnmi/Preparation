#pragma once

#include "MathBase.h"

namespace Math
{

	class int2
	{
	public:
		// default constructor / destructor
		//
		int2();
		~int2();

		// ctors with params
		//
		int2( int nx, int ny );

		// copy ctor and assign
		//
		int2( const int2& other );
		int2& operator=( const int2& other );

	public:
		// values for vector
		//
		int x, y;
	};
}