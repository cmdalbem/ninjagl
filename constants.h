// constants.h
//
// General purpose macros and constants.
//

#pragma once

#include "vector3f.h"
#include "vector4f.h"


#ifndef GLUT_WHEEL_UP
#define GLUT_WHEEL_MIDDLE 1
#define GLUT_WHEEL_UP   3
#define GLUT_WHEEL_DOWN 4
#endif


// Types
struct Vertex3f
{
	vector3f pos, normal;
	float color[3];
	float s, t;
};
struct Triangle3f
{
	Vertex3f v[3];
	vector3f normal;
};

struct Vertex4f
{
	vector4f pos, normal;
	float color[3];
	float s, t;
};
struct Triangle4f
{
	Vertex4f v[4];
	vector4f normal;
};
	

// Macros
#define DEBUG_VAR(x) cout << #x << ": " << x << endl

#define RGB(x) (float)(x)/(255)
#define PRINTVAR(x) cout << #x << " = " << x << endl
#define RAD(x) ((x)*(M_PI))/(180.)
#define DEGREES(x) ((x)*(180.))/(M_PI)

// Keyboard keys
enum{
	K_ESC		= 27,
	K_SPACEBAR	= 32
};
	



