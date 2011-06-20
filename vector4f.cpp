#include <stdio.h>

#include "vector4f.h"

vector4f::vector4f( float x_, float y_, float z_, float w_ )
{
	set(x_,y_,z_,w_);
}

void vector4f::set( float x_, float y_, float z_, float w_)
{
    x = x_;
    y = y_;
    z = z_;
    w = w_;
}

float vector4f::length( void )
{
    return( (float)sqrt( x * x + y * y + z * z + w * w) );
}

void vector4f::normalize( void )
{
    float fLength = length();

    x = x / fLength;
    y = y / fLength;
    z = z / fLength;
    w = w / fLength;
}

vector3f vector4f::toVector3f()
{
	return vector3f(x,y,z);	
}

// Operators...

vector4f vector4f::operator + ( const vector4f &other )
{
    vector4f vResult(0.0f, 0.0f, 0.0f, 0.0f);

    vResult.x = x + other.x;
    vResult.y = y + other.y;
    vResult.z = z + other.z;
    vResult.w = w + other.w;

    return vResult;
}

vector4f vector4f::operator + ( void ) const
{
    return *this;
}

vector4f vector4f::operator - ( const vector4f &other )
{
    vector4f vResult(0.0f, 0.0f, 0.0f, 0.0f);

    vResult.x = x - other.x;
    vResult.y = y - other.y;
    vResult.z = z - other.z;
    vResult.w = w - other.w;

    return vResult;
}

vector4f vector4f::operator - ( void ) const
{
    vector4f vResult(-x, -y, -z, -w);

    return vResult;
}

vector4f vector4f::operator * ( const vector4f &other )
{
    vector4f vResult(0.0f, 0.0f, 0.0f, 0.0f);

    vResult.x = x * other.x;
    vResult.y = y * other.y;
    vResult.z = z * other.z;
    vResult.w = w * other.w;

    return vResult;
}

vector4f vector4f::operator * ( const float scalar )
{
    vector4f vResult(0.0f, 0.0f, 0.0f, 0.0f);

    vResult.x = x * scalar;
    vResult.y = y * scalar;
    vResult.z = z * scalar;
    vResult.w = w * scalar;

    return vResult;
}

vector4f operator * ( const float scalar, const vector4f &other )
{
    vector4f vResult(0.0f, 0.0f, 0.0f, 0.0f);

    vResult.x = other.x * scalar;
    vResult.y = other.y * scalar;
    vResult.z = other.z * scalar;
    vResult.w = other.w * scalar;

    return vResult;
}

vector4f vector4f::operator / ( const vector4f &other )
{
    vector4f vResult(0.0f, 0.0f, 0.0f, 0.0f);

    vResult.x = x / other.x;
    vResult.y = y / other.y;
    vResult.z = z / other.z;
    vResult.w = w / other.w;

    return vResult;
}

vector4f vector4f::operator / (float value)
{
    vector4f vResult(0.0f, 0.0f, 0.0f, 0.0f);
	if(value==0)
		printf("ERROR!!!\n");
    vResult.x = x / value;
    vResult.y = y / value;
    vResult.z = z / value;
    vResult.w = w / value;

    return vResult;
}

vector4f& vector4f::operator = ( const vector4f &other )
{
    x = other.x;
    y = other.y;
    z = other.z;
    w = other.w;

    return *this;
}

vector4f& vector4f::operator += ( const vector4f &other )
{
    x += other.x;
    y += other.y;
    z += other.z;
    w += other.w;

    return *this;
}

vector4f& vector4f::operator -= ( const vector4f &other )
{
    x -= other.x;
    y -= other.y;
    z -= other.z;
    w -= other.w;

    return *this;
}

// Static utility methods...

float distance( const vector4f &v1,  const vector4f &v2  )
{
    float dx = v1.x - v2.x;
    float dy = v1.y - v2.y;
    float dz = v1.z - v2.z;

    return (float)sqrt( dx * dx + dy * dy + dz * dz );
}

float dotProduct( const vector4f &v1,  const vector4f &v2 )
{
    return( v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w );
}

vector4f crossProduct( const vector4f &v1,  const vector4f &v2 )
{
    vector4f vCrossProduct;

    vCrossProduct.x =  v1.y * v2.z - v1.z * v2.y;
    vCrossProduct.y = -v1.x * v2.z + v1.z * v2.x;
    vCrossProduct.z =  v1.x * v2.y - v1.y * v2.x;

    return vCrossProduct;
}
