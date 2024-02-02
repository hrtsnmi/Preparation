#pragma once

#include "Math\float3.h"

namespace Math
{
	class Ray
	{
	public:
		// default {ctor} / {dtor}
		Ray();
		~Ray();

		// copy and assigment operator
		//
		Ray( const Ray& other );
		Ray& operator=( const Ray& other );

		// setter / getter ray direction
		//
		void setDirection( const float3& dir );
		const float3& getDirection()const;

		// setter / getter ray position
		//
		void setPosition( const float3& pos );
		const float3& getPosition()const;

		float3 PointAt( float t )const;

	protected:
		// ray direction
		//
		float3 _Direction;

		// start point for ray
		//
		float3 _Position;

	};
}