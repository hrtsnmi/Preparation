#include "pch.h"

#include "float3.h"



using namespace Math;

const float3 float3::kZero{ 0.0f,0.0f,0.0f };
const float3 float3::kOne{ 1.0f,1.0f,1.0f };

const float3 float3::kAxisX{ 1.0f,0.0f,0.0f };
const float3 float3::kAxisY{ 0.0f,1.0f,0.0f };
const float3 float3::kAxisZ{ 0.0f,0.0f,1.0f };


float3::float3()
	: x( 0.0f )
	, y( 0.0f )
	, z( 0.0f )
{
}

float3::~float3()
{
}


float3::float3( float fx, float fy, float fz )
	: x( fx )
	, y( fy )
	, z( fz )
{
}

float3::float3( const float3 & other )
	: x( other.x )
	, y( other.y )
	, z( other.z )
{
}

float3 & float3::operator=( const float3 & other )
{
	if( this != &other )
	{
		this->x = other.x;
		this->y = other.y;
		this->z = other.z;
	}
	return *this;
}

bool float3::operator==( const float3 & other ) const
{
	return this->x == other.x && this->y == other.y && this->z == other.z;
}

bool float3::operator!=( const float3 & other ) const
{
	return !this->operator==( other );
}

float3 & float3::operator+=( const float3 & other )
{
	this->x += other.x;
	this->y += other.y;
	this->z += other.z;
	return *this;
}

float3 & float3::operator+=( const float fValue )
{
	this->x += fValue;
	this->y += fValue;
	this->z += fValue;
	return *this;
}

float3 & float3::operator-=( const float3 & other )
{
	this->x -= other.x;
	this->y -= other.y;
	this->z -= other.z;
	return *this;
}

float3 & float3::operator-=( const float fValue )
{
	this->x -= fValue;
	this->y -= fValue;
	this->z -= fValue;
	return *this;
}

float3 & float3::operator/=( const float3 & other )
{
	this->x /= other.x;
	this->y /= other.y;
	this->z /= other.z;
	return *this;
}

float3 & float3::operator/=( const float fValue )
{
	this->x /= fValue;
	this->y /= fValue;
	this->z /= fValue;
	return *this;
}

float3 & float3::operator*=( const float3 & other )
{
	this->x *= other.x;
	this->y *= other.y;
	this->z *= other.z;
	return *this;
}

float3 & float3::operator*=( const float fValue )
{
	this->x *= fValue;
	this->y *= fValue;
	this->z *= fValue;
	return *this;
}


float3 float3::operator+( const float3 & other )const
{
	return float3( this->x + other.x, this->y + other.y, this->z + other.z );
}

float3 float3::operator+( const float fValue )const
{
	return float3( this->x + fValue, this->y + fValue, this->z + fValue );
}


float3 float3::operator-( const float3 & other )const
{
	return float3( this->x - other.x, this->y + -other.y, this->z - other.z );
}

float3 float3::operator-( const float fValue )const
{
	return float3( this->x - fValue, this->y - fValue, this->z - fValue );
}

float3 float3::operator/( const float3 & other )const
{
	return float3( this->x / other.x, this->y / other.y, this->z / other.z );
}

float3 float3::operator/( const float fValue )const
{
	return float3( this->x / fValue, this->y / fValue, this->z / fValue );
}

float3 float3::operator*( const float3 & other )const
{
	return float3( this->x * other.x, this->y * other.y, this->z * other.z );
}

float3 float3::operator*( const float fValue )const
{
	return float3( this->x * fValue, this->y * fValue, this->z * fValue );
}

float3 Math::operator*( float value, const float3 & f3 )
{
	return f3 * value;
}

float3 Math::float3::operator-() const
{
	return this->Invert();
}

float3::operator float*( )
{
	return &this->x;
}

float3::operator const float*( ) const
{
	return &this->x;
}

float & float3::operator[]( unsigned int uiIndex )
{
	return ( &this->x )[ uiIndex ];
}

const float & float3::operator[]( unsigned int uiIndex ) const
{
	return ( &this->x )[ uiIndex ];
}

float float3::Length()const
{
	return sqrtf( Dot( *this, *this ) );
}

float float3::LengthSq()const
{
	return Dot( *this, *this );
}

float3 float3::Norm()const
{
	float fLenght = this->Length();
	if( fLenght )
	{
		fLenght = 1.0f / fLenght;
	}

	return *this * fLenght;
}

float3 float3::Invert()const
{
	return float3( -this->x,
		-this->y,
		-this->z );
}

float float3::Dot( const float3 & fa, const float3& fb )
{
	return fa.x * fb.x + fa.y * fb.y + fa.z * fb.z;
}
