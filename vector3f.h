//-----------------------------------------------------------------------------
//           Name: vector3f.h
//         Author: Kevin Harris
//  Last Modified: 02/01/05
//    Description: OpenGL compatible utility class for a 3D vector of floats
//                 NOTE: This class has been left unoptimized for readability.
//-----------------------------------------------------------------------------

#pragma once

#include <cmath>

class vector3f
{
	public:

		float x;
		float y;
		float z;

		vector3f(void)
		{
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;
		}

		vector3f(float x_, float y_, float z_);
		void set(float x_, float y_, float z_);
		float length(void);
		void normalize(void);

		// Operators...
		vector3f operator + (const vector3f &other);
		vector3f operator - (const vector3f &other);
		vector3f operator * (const vector3f &other);
		vector3f operator / (const vector3f &other);

		vector3f operator * (const float scalar);
		friend vector3f operator * (const float scalar, const vector3f &other);
		
		vector3f& operator = (const vector3f &other);
		vector3f& operator += (const vector3f &other);
		vector3f& operator -= (const vector3f &other);

		vector3f operator + (void) const;
		vector3f operator - (void) const;
};

float distance(const vector3f &v1, const vector3f &v2);
float dotProduct(const vector3f &v1,  const vector3f &v2 );
vector3f crossProduct(const vector3f &v1, const vector3f &v2);
