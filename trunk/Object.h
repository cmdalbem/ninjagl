// Object Class
//
// Adapted from SiNoS (http://sinos.googlecode.com)
//

#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <GL/glut.h>

#include "Material.h"

using namespace std;


class Object
{		
	public:				
		//---------------------------- ATTRIBUTES
		GLfloat pos[3],		// position x, y and z of the Object
				rot[3],		// rotation arguments on Rotatefs functions called on Draw
				size[3];	// scale arguments on Scalefs functions called on Draw
			
		vector<Material>	materials;
		vector<Triangle>	tris;
		Point3df			forceColor;
		
		Point3df			maxPoint, minPoint, centerPoint;

		//---------------------------- CONSTRUCTORS
		Object();
		Object(string filename);
		void init();
		
		//---------------------------- DESTRUCTORS
		~Object();
		
		//---------------------------- GETTERS & SETTERS
		void resetPosition();
		void resetRotation();
		void resetSize();
		
		//---------------------------- OTHER METHODS
		
		int		readFromFile(string filename);
		void	draw( bool isColored=false ) const;
		void	drawNormals( int mult ) const;
		void	drawBoundingBox() const;
		
	
	private:
		void	drawBegin() const;
		void	drawEnd() const;
};
