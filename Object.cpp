// Object Class
//
// Adapted from SiNoS (http://sinos.googlecode.com)
//

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>

#include "constants.h"
#include "Object.h"

using namespace std;


//------------------------------------------------------------ CONSTRUCTORS

Object::Object()
{
	init();
}

Object::Object(string filename)
{
	init();	
	readFromFile(filename);
}

void Object::init()
{
	resetPosition();	
	resetRotation();
	resetSize();
}

//------------------------------------------------------------ DESTRUCTORS

Object::~Object() {}

//------------------------------------------------------------ GETTERS & SETTERS

void Object::resetPosition()
{
	pos[0] = 0;
	pos[1] = 0;
	pos[2] = 0;
}

void Object::resetRotation()
{
	rot[0] = 0;
	rot[1] = 0;
	rot[2] = 0;
}

void Object::resetSize()
{	
	size[0] = 1;
	size[1] = 1;
	size[2] = 1;
}



//------------------------------------------------------------ OTHER METHODS


int Object::readFromFile( string filename )
{
	int nmaterials, ntriangles, materialIndex;
	char ch;
	
	FILE* fp = fopen(filename.c_str(),"r");
	if (fp==NULL)
		return 1;
		
	fscanf(fp, "%c", &ch);
	while(ch!= '\n') // skip the first line – object’s name
		fscanf(fp, "%c", &ch);

	// read triangles and material count
	fscanf(fp,"# triangles = %d\n", &ntriangles);
	fscanf(fp,"Material count = %d\n", &nmaterials);
	ntriangles = ntriangles;
	
	//materials.resize(nmaterials);
	
	// read materials
	for (int i=0; i<nmaterials; i++) {
		fscanf(fp, "ambient color %f %f %f\n", &(material.ambient[0]), &(material.ambient[1]), &(material.ambient[2]));
		fscanf(fp, "diffuse color %f %f %f\n", &(material.diffuse[0]), &(material.diffuse[1]), &(material.diffuse[2]));
		fscanf(fp, "specular color %f %f %f\n", &(material.specular[0]), &(material.specular[1]), &(material.specular[2]));
		fscanf(fp, "material shine %f\n", &(material.shininess));
	}
	
	char chasTex[8];
	fscanf(fp,"Texture = %s\n", chasTex);
	hasTex = (chasTex[0] == 'Y') ? true : false;
	
	// skip documentation line
	fscanf(fp, "%c", &ch);
	while(ch!= '\n')
		fscanf(fp, "%c", &ch);
	
	printf ("Reading in %s (%d%s triangles)...\n", filename.c_str(), ntriangles, hasTex?" texturized":"");
	
	// set size of triangles vector
	tris.resize(ntriangles);
	
	// read triangles
	maxPoint = {-9999,-9999,-9999};
	minPoint = {9999,9999,9999};
	for (int i=0; i<ntriangles; i++) {
		int k = 0;
			if(hasTex)
				fscanf(fp, "v0 %f %f %f %f %f %f %d %f %f\n",
							&(tris[i].v[k].pos.x), &(tris[i].v[k].pos.y), &(tris[i].v[k].pos.z),
							&(tris[i].v[k].normal.x), &(tris[i].v[k].normal.y), &(tris[i].v[k].normal.z), //ignored
							&(materialIndex), &(tris[i].s), &(tris[i].t));
			else
				fscanf(fp, "v0 %f %f %f %f %f %f %d\n",
							&(tris[i].v[k].pos.x), &(tris[i].v[k].pos.y), &(tris[i].v[k].pos.z),
							&(tris[i].v[k].normal.x), &(tris[i].v[k].normal.y), &(tris[i].v[k].normal.z), //ignored
							&(materialIndex));
			tris[i].v[k].color[0] = material.diffuse[0];
			tris[i].v[k].color[1] = material.diffuse[1];
			tris[i].v[k].color[2] = material.diffuse[2];
			if(tris[i].v[k].pos.x > maxPoint.x) maxPoint.x = tris[i].v[k].pos.x;
			if(tris[i].v[k].pos.y > maxPoint.y) maxPoint.y = tris[i].v[k].pos.y;
			if(tris[i].v[k].pos.z > maxPoint.z) maxPoint.z = tris[i].v[k].pos.z;
			if(tris[i].v[k].pos.x < minPoint.x) minPoint.x = tris[i].v[k].pos.x;
			if(tris[i].v[k].pos.y < minPoint.y) minPoint.y = tris[i].v[k].pos.y;
			if(tris[i].v[k].pos.z < minPoint.z) minPoint.z = tris[i].v[k].pos.z;
		k = 1;
			if(hasTex)
				fscanf(fp, "v1 %f %f %f %f %f %f %d %f %f\n",
							&(tris[i].v[k].pos.x), &(tris[i].v[k].pos.y), &(tris[i].v[k].pos.z),
							&(tris[i].v[k].normal.x), &(tris[i].v[k].normal.y), &(tris[i].v[k].normal.z), //ignored
							&(materialIndex), &(tris[i].s), &(tris[i].t));
			else
				fscanf(fp, "v1 %f %f %f %f %f %f %d\n",
							&(tris[i].v[k].pos.x), &(tris[i].v[k].pos.y), &(tris[i].v[k].pos.z),
							&(tris[i].v[k].normal.x), &(tris[i].v[k].normal.y), &(tris[i].v[k].normal.z), //ignored
							&(materialIndex));
			tris[i].v[k].color[0] = material.diffuse[0];
			tris[i].v[k].color[1] = material.diffuse[1];
			tris[i].v[k].color[2] = material.diffuse[2];
			if(tris[i].v[k].pos.x > maxPoint.x) maxPoint.x = tris[i].v[k].pos.x;
			if(tris[i].v[k].pos.y > maxPoint.y) maxPoint.y = tris[i].v[k].pos.y;
			if(tris[i].v[k].pos.z > maxPoint.z) maxPoint.z = tris[i].v[k].pos.z;
			if(tris[i].v[k].pos.x < minPoint.x) minPoint.x = tris[i].v[k].pos.x;
			if(tris[i].v[k].pos.y < minPoint.y) minPoint.y = tris[i].v[k].pos.y;
			if(tris[i].v[k].pos.z < minPoint.z) minPoint.z = tris[i].v[k].pos.z;
		k = 2;
			if(hasTex)
				fscanf(fp, "v2 %f %f %f %f %f %f %d %f %f\n",
							&(tris[i].v[k].pos.x), &(tris[i].v[k].pos.y), &(tris[i].v[k].pos.z),
							&(tris[i].v[k].normal.x), &(tris[i].v[k].normal.y), &(tris[i].v[k].normal.z), //ignored
							&(materialIndex), &(tris[i].s), &(tris[i].t));
			else
				fscanf(fp, "v2 %f %f %f %f %f %f %d\n",
							&(tris[i].v[k].pos.x), &(tris[i].v[k].pos.y), &(tris[i].v[k].pos.z),
							&(tris[i].v[k].normal.x), &(tris[i].v[k].normal.y), &(tris[i].v[k].normal.z), //ignored
							&(materialIndex));
			tris[i].v[k].color[0] = material.diffuse[0];
			tris[i].v[k].color[1] = material.diffuse[1];
			tris[i].v[k].color[2] = material.diffuse[2];
			if(tris[i].v[k].pos.x > maxPoint.x) maxPoint.x = tris[i].v[k].pos.x;
			if(tris[i].v[k].pos.y > maxPoint.y) maxPoint.y = tris[i].v[k].pos.y;
			if(tris[i].v[k].pos.z > maxPoint.z) maxPoint.z = tris[i].v[k].pos.z;
			if(tris[i].v[k].pos.x < minPoint.x) minPoint.x = tris[i].v[k].pos.x;
			if(tris[i].v[k].pos.y < minPoint.y) minPoint.y = tris[i].v[k].pos.y;
			if(tris[i].v[k].pos.z < minPoint.z) minPoint.z = tris[i].v[k].pos.z;;
					
		fscanf(fp, "face normal %f %f %f\n",
					&(tris[i].normal.x), &(tris[i].normal.y),
					&(tris[i].normal.z));
		
		tris[i].normal = -crossProduct( (tris[i].v[1].pos-tris[i].v[0].pos), (tris[i].v[2].pos-tris[i].v[1].pos) );
		tris[i].normal.normalize();
	}
	centerPoint.x = (maxPoint.x + minPoint.x)/2;
	centerPoint.y = (maxPoint.y + minPoint.y)/2;
	centerPoint.z = (maxPoint.z + minPoint.z)/2;
	
	fclose(fp);
	
	return 0;
}


void Object::drawBegin() const 
{
	glPushMatrix();
	
	glTranslated(pos[0], pos[1], pos[2]);
	
	glRotatef(rot[1],0,1,0);
	glRotatef(rot[0],1,0,0);
	glRotatef(rot[2],0,0,1);
	
	glScalef(size[0], size[1], size[2]);
	
	material.apply();
}

void Object::drawEnd() const
{
	glPopMatrix();
}

void Object::draw( bool isColored ) const
{
	drawBegin();
	
		glBegin(GL_TRIANGLES);
		if(isColored)
			glColor3f( forceColor[0],forceColor[1],forceColor[2] );
		for(unsigned int i=0; i<tris.size(); i++) {
			for(int k=0; k<3; k++) {
				if(!isColored)
					glColor3f( tris[i].v[k].color[0],
								tris[i].v[k].color[1],
								tris[i].v[k].color[2]); 
				glNormal3f(tris[i].v[k].normal.x, tris[i].v[k].normal.y, tris[i].v[k].normal.z); 
				if(hasTex)
					glTexCoord2f(tris[i].s, tris[i].t);
				glVertex3f(tris[i].v[k].pos.x, tris[i].v[k].pos.y, tris[i].v[k].pos.z);
			}
		}
		glEnd();
	
	drawEnd();
}

void Object::drawNormals( int mult ) const
{
	drawBegin();
	
		glColor3f( 1.0f, 1.0f, 1.0f );
		glDisable(GL_LIGHTING);
		glBegin(GL_LINES);
		for(unsigned int i=0; i<tris.size(); i++) {
			for(int k=0; k<3; k++) {
				glVertex3f(tris[i].v[k].pos.x, tris[i].v[k].pos.y, tris[i].v[k].pos.z);
				glVertex3f(tris[i].v[k].pos.x + mult*tris[i].v[k].normal.x,
							tris[i].v[k].pos.y + mult*tris[i].v[k].normal.y,
							tris[i].v[k].pos.z + mult*tris[i].v[k].normal.z);
			}
		}
		glEnd();
		glEnable(GL_LIGHTING);
	
	drawEnd();
}

void Object::drawBoundingBox() const
{
	drawBegin();
	
		glColor3f( 1.0f, 1.0f, 1.0f );
		glDisable(GL_LIGHTING);
		// ceiling
		glBegin(GL_LINE_LOOP);
			glVertex3f(minPoint.x, maxPoint.y, maxPoint.z);
			glVertex3f(maxPoint.x, maxPoint.y, maxPoint.z);
			glVertex3f(maxPoint.x, maxPoint.y, minPoint.z);
			glVertex3f(minPoint.x, maxPoint.y, minPoint.z);
		glEnd();	
		// roof
		glBegin(GL_LINE_LOOP);
			glVertex3f(minPoint.x, minPoint.y, maxPoint.z);
			glVertex3f(maxPoint.x, minPoint.y, maxPoint.z);
			glVertex3f(maxPoint.x, minPoint.y, minPoint.z);
			glVertex3f(minPoint.x, minPoint.y, minPoint.z);
		glEnd();
		// walls
		glBegin(GL_LINES);
			glVertex3f(minPoint.x, minPoint.y, maxPoint.z);
			glVertex3f(minPoint.x, maxPoint.y, maxPoint.z);
			
			glVertex3f(maxPoint.x, minPoint.y, minPoint.z);
			glVertex3f(maxPoint.x, maxPoint.y, minPoint.z);
			
			glVertex3f(minPoint.x, minPoint.y, minPoint.z);
			glVertex3f(minPoint.x, maxPoint.y, minPoint.z);
			
			glVertex3f(maxPoint.x, minPoint.y, maxPoint.z);
			glVertex3f(maxPoint.x, maxPoint.y, maxPoint.z);
		glEnd();
		glEnable(GL_LIGHTING);
	
	drawEnd();
}
