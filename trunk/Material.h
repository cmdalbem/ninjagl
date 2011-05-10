// Material Class
//
// Adapted from SiNoS (http://sinos.googlecode.com)
//

#pragma once

#include <iostream>
#include <string>

#include <GL/glut.h>


// Default values for Materials Attributes
const float default_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
const float default_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
const float default_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
const float default_shininess = 0;



class Material
{
    public:
    	//---------------------------- ATTRIBUTES
		float ambient[3],
			diffuse[3],
			specular[3],
			shininess;
				
    	//---------------------------- CONSTRUCTORS
    	Material();
    
    	//---------------------------- DESTRUCTORS
    	~Material();
    	
    	//---------------------------- GETTERS & SETTERS
    	
    	void setAmbient(float r, float g, float b);
    	void setDiffuse(float r, float g, float b);
    	void setSpecular(float r, float g, float b);
    	void setShininess(float rgba);
    	
    	//---------------------------- OTHER METHODS
    	void		apply()		const;
    	void		unapply()	const;
    	void 		resetMaterial();
};
