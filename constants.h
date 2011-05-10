// constants.h
//
// General purpose macros and constants.
//

#pragma once


#ifndef GLUT_WHEEL_UP
#define GLUT_WHEEL_MIDDLE 1
#define GLUT_WHEEL_UP   3
#define GLUT_WHEEL_DOWN 4
#endif


// Types
struct Point3di
{
	int x,y,z;
};

struct Point3df
{
	float x,y,z;
};
struct Vertex
{
	Point3df pos, normal;
	float color[3];
};
struct Triangle
{
	Vertex v[3];
	Point3df normal;
};
	

// Macros
#define RGB(x) (float)(x)/(255)
#define PRINTVAR(x) cout << #x << " = " << x << endl
#define RAD(x) ((x)*(M_PI))/(180.)
#define DEGREES(x) ((x)*(180.))/(M_PI)

// Keyboard keys
enum{
	K_ESC		= 27,
	K_SPACEBAR	= 32
};
	



