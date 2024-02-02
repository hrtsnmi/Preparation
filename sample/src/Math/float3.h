#pragma once

#include "Math/MathBase.h"

namespace Math
{

	class float3
	{
	public:
		static const float3 kZero;	// { 0.0f, 0.0f, 1.0f }
		static const float3 kOne;	// { 1.0f, 1.0f, 1.0f }

		static const float3 kAxisX;	// { 1.0f, 0.0f, 0.0f }
		static const float3 kAxisY;	// { 0.0f, 1.0f, 0.0f }
		static const float3 kAxisZ; // { 0.0f, 0.0f, 1.0f }
	public:
		// default constructor / destructor
		//
		float3();
		~float3();

		// ctors with params
		//
		float3( float fx, float fy, float fz );

		// copy ctor and assign
		//
		float3( const float3& other );
		float3& operator=( const float3& other );

		// operators == and !=
		//
		bool operator==( const float3& other )const;
		bool operator!=( const float3& other )const;


		// operator += for another float3 and for float value
		//
		float3& operator+=( const float3& other );
		float3& operator+=( const float fvalue );


		// operator -= for another float3 and for float value
		//
		float3& operator-=( const float3& other );
		float3& operator-=( const float fvalue );


		// operator /= by another float3 and for float value
		//
		float3& operator/=( const float3& other );
		float3& operator/=( const float fvalue );


		// operator *= by another float3 and for float value
		//
		float3& operator*=( const float3& other );
		float3& operator*=( const float fvalue );

		// operator + for another float3 and for float value
		//
		float3 operator+( const float3& other )const;
		float3 operator+( const float fvalue )const;
		
		// operator - for another float3 and for float value
		//
		float3 operator-( const float3& other )const;
		float3 operator-( const float fvalue )const;
		
		// operator / by another float3 and for float value
		//
		float3 operator/( const float3& other )const;
		float3 operator/( const float fvalue )const;

		// operator * by another float3 and for float value
		//
		float3 operator*( const float3& other )const;
		float3 operator*( const float fvalue )const;

		// reverse operator
		// for example: 2.0f*float3 
		//
		friend float3 operator*( float value, const float3& f3 );
		

		// reverse operation
		//
		float3 operator-() const;

		// converting data class to float*
		//
		operator float*( );
		operator const float*( )const;

		// getting element by id
		//
		float& operator[] ( unsigned int uiIndex );
		const float& operator[]( unsigned int uiIndex )const;

		// getting lenght vector
		//
		float Length()const;
		float LengthSq()const;

		// normalizing float3
		//
		float3 Norm()const;

		// inverting direction
		//
		float3 Invert( )const;

		// --------------------- General functional ---------------------//
		// getting dot-product of two vector
		//
		static float Dot( const float3& fa, const float3& fb );


	public:
		// values for vector
		//
		float x;
		float y;
		float z;
	};
}