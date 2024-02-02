#include "pch.h"

#include "int2.h"

using namespace Math;

int2::int2()
	: x( 0 )
	, y( 0 )
{
}

int2::~int2()
{
}

int2::int2( int nx, int ny )
	: x( nx )
	, y( ny )
{
}


int2::int2( const int2 & other )
	: x( other.x )
	, y( other.y )
{
}

int2 & int2::operator=( const int2 & other )
{
	if( this != &other )
	{
		this->x = other.x;
		this->y = other.y;
	}
	return *this;
}
