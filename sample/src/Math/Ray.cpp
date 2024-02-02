#include "pch.h"

#include "Ray.h"

using namespace Math;
Ray::Ray()
{
}

Ray::~Ray()
{
}

Ray::Ray( const Ray & other )
{
	this->_Direction = other._Direction;
	this->_Position = other._Position;
}

Ray & Ray::operator=( const Ray & other )
{
	if( this != &other )
	{
		this->_Direction = other._Direction;
		this->_Position = other._Position;
	}
	return *this;
}

void Ray::setDirection( const float3 & dir )
{
	this->_Direction = dir;
}

const float3 & Ray::getDirection() const
{
	return this->_Direction;
}

void Ray::setPosition( const float3 & pos )
{
	this->_Position = pos;
}

const float3 & Ray::getPosition() const
{
	return this->_Position;
}

Math::float3 Math::Ray::PointAt( float t ) const
{
	return this->_Position + this->_Direction * t;
}
