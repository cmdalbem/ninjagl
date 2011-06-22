//
//
//			vector4f.h ADAPTED FROM
//
//-----------------------------------------------------------------------------
//           Name: vector4f.h
//         Author: Kevin Harris
//  Last Modified: 02/01/05
//    Description: OpenGL compatible utility class for a 3D vector of floats
//                 NOTE: This class has been left unoptimized for readability.
//-----------------------------------------------------------------------------

#pragma once


#include <cmath>

#include "vector3f.h"


class vector4f
{
	public:

		float x;
		float y;
		float z;
		float w;

		vector4f(void)
		{
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;
			w = 0.0f;
		}

		vector4f(float x_, float y_, float z_, float w_);
		void set(float x_, float y_, float z_, float w_);
		float length(void);
		void normalize(void);
		vector3f toVector3f();

		// Operators...
		vector4f operator + (const vector4f &other);
		vector4f operator - (const vector4f &other);
		vector4f operator * (const vector4f &other);
		vector4f operator / (const vector4f &other);
		vector4f operator / (float value);

		vector4f operator * (const float scalar);
		friend vector4f operator * (const float scalar, const vector4f &other);
		
		vector4f& operator = (const vector4f &other);
		vector4f& operator += (const vector4f &other);
		vector4f& operator -= (const vector4f &other);

		vector4f operator + (void) const;
		vector4f operator - (void) const;
};

// Static utility methods
float distanceV(const vector4f &v1, const vector4f &v2);
float dotProduct(const vector4f &v1,  const vector4f &v2 );
vector4f crossProduct(const vector4f &v1, const vector4f &v2);
