// Material Class
//
// Adapted from SiNoS (http://sinos.googlecode.com)
//

#include <iostream>
#include "Material.h"


//------------------------------------------------------------ CONSTRUCTORS

Material::Material()
{
	resetMaterial();
}

//------------------------------------------------------------ DESTRUCTORS

Material::~Material()
{
	// This method is purposely blank
}

//------------------------------------------------------------ GETTERS & SETTERS


void Material::setAmbient(float r, float g, float b)
{
	ambient[0] = r;
	ambient[1] = g;
	ambient[2] = b;
}
void Material::setDiffuse(float r, float g, float b)
{
	diffuse[0] = r;
	diffuse[1] = g;
	diffuse[2] = b;
}
void Material::setSpecular(float r, float g, float b)
{
	specular[0] = r;
	specular[1] = g;
	specular[2] = b;
}

void Material::setShininess(float rgba)
{
	shininess = rgba;
}


//-------------------------------------------------------- OTHER METHODS

void Material::apply() const
{
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	//glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	//glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	//glMaterialfv(GL_FRONT, GL_SHININESS, &shininess);
}

void Material::unapply() const
{
	glMaterialfv(GL_FRONT, GL_DIFFUSE, default_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, default_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, default_ambient);
	glMaterialfv(GL_FRONT, GL_SHININESS, &default_shininess);
}

void Material::resetMaterial()
{
	setAmbient( default_ambient[0], 
				default_ambient[1],
				default_ambient[2]);
	
	setDiffuse( default_diffuse[0],
				default_diffuse[1],
				default_diffuse[2]);
	
	setSpecular(default_specular[0],
				default_specular[1],
				default_specular[2]);
				
	setShininess(default_shininess);
}
