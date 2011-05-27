#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <vector>
#include <string.h>

#include <GL/glui.h>
#include <GL/gl.h>
#include <GL/glut.h>

#include "Object.h"
#include "constants.h"
#include "matrix4x4f.h"
						 
using namespace std;


// GLOBALS ////////////////////////////////////////////////////////////

#define 		BG_COLOR 0.2, 0.2, 0.2
#define 		OSD_LINES 2
#define			WINDOWSX 100
#define			WINDOWSY 300
			
int				mainWindow, ninjaWindow;
Object			object;
vector<Triangle4f> objTris;

// Mouse-Keyboard
static int      xold, yold;		
static int	    left_click=GLUT_UP, right_click=GLUT_UP, middle_click=GLUT_UP;
static bool     heldCtrl=false, heldShift=false;

long int		frameCounter, frameCounter2, fps, fps2;
char 			osd[OSD_LINES][256], osd2[OSD_LINES][256];
int				width=620, height=620;
vector3f		cameraPos, cameraU, cameraV, cameraN;

// GUI controlled
float			fovy=60, fovx=60, clipNear=0.1, clipFar=3000;
int				drawOpt=1, cameraMoveOpt, orientationOpt, shadeOpt;
int				enableFixedLight=1, enableRotLight=1, enableCulling=0, 
				enableDrawNormals=0, enableDrawBoundingBox=0, enableColoredDraw=0,
				enableLight=0;
vector3f		forceColor={1,1,1};
char			fileName[256];


// PROTOTYPES //////////////////////////////////////////////////////////

void resetCamera( int nil=0 );
void updateSettings( int nil=0 );
void loadModel( int nil=0 );


// OPENGL //////////////////////////////////////////////////////////////

void drawObjects()
{
	// Model Drawing
	switch(drawOpt) {
		case 0:	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL ); break;
		case 1:	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ); break;
		case 2:	glPolygonMode( GL_FRONT_AND_BACK, GL_POINT ); break;
	}
	
	if(enableColoredDraw) {
		object.forceColor = forceColor;
		object.draw(true);
	}
	else
		object.draw(false);
	if(enableDrawBoundingBox)
		object.drawBoundingBox();
	if(enableDrawNormals)
		object.drawNormals(20);
}

void lights()
{			
	GLfloat pos[] = { 0 , 200 , 0 , 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	
	static float posIter = 0;
	GLfloat pos2[] = { (float)sin(posIter)*300 , 100 , (float)cos(posIter)*150 , 1.0f };
	glLightfv(GL_LIGHT1, GL_POSITION, pos2);
	posIter += 0.01;
}

void camera()
{
	gluLookAt( 	cameraPos.x, cameraPos.y, cameraPos.z,
				cameraPos.x+cameraN.x, cameraPos.y+cameraN.y, cameraPos.z+cameraN.z,
				cameraV.x, cameraV.y, cameraV.z);
}

void drawOsd()
/* Draws On-Screen Display */
/* Adapted from http://www.opengl.org/resources/code/samples/glut_examples/examples/bitfont.c */
{
	glDisable(GL_LIGHTING);

		glColor3f(1.,1.,1.);

		glRasterPos3f(-0.55,0.52,-1);
		for( int ch = 0; ch < (int)strlen(osd[0]); ch++) 
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, osd[0][ch]);

		glRasterPos3f(-0.55,0.52-0.04,-1);
		for( int ch = 0; ch < (int)strlen(fileName); ch++) 
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, fileName[ch]);
   
	enableLight ? glEnable(GL_LIGHTING) : glDisable(GL_LIGHTING);
}

void reshape(int w, int h) {
	
	glutSetWindow(mainWindow);
	
	width = w;
	height = h;
	
	glViewport(0, 0, (GLsizei) width, (GLsizei) height);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective (fovy, tan(fovx*M_PI/360)/tan(fovy*M_PI/360), clipNear, clipFar);
	
	glMatrixMode(GL_MODELVIEW);
}

void display () {
// The Original Display Function
	
	glutSetWindow(mainWindow);
	
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
    glLoadIdentity ();
    
		drawOsd();
		camera();
		lights();
		drawObjects();
	
	glutSwapBuffers();
	glutPostRedisplay();
	frameCounter++;
}


// NINJAGL /////////////////////////////////////////////////////////////

matrix4x4f modelviewMatrix, projectionMatrix, viewportMatrix;


void updateModelviewMatrix()
{
	matrix4x4f *m = &modelviewMatrix;
	m->m[0]=cameraU.x;	m->m[4]=cameraU.y;	m->m[8]=cameraU.z;		m->m[12]=-dotProduct(cameraPos,cameraU);
    m->m[1]=cameraV.x;	m->m[5]=cameraV.y;	m->m[9]=cameraV.z;		m->m[13]=-dotProduct(cameraPos,cameraV);
    m->m[2]=-cameraN.x;	m->m[6]=-cameraN.y;	m->m[10]=-cameraN.z;	m->m[14]=dotProduct(cameraPos,cameraN);
    m->m[3]=0; 			m->m[7]=0; 			m->m[11]=0;				m->m[15]=1;	
}

void updateProjectionMatrix()
{
	float n = clipNear,
		  f = clipFar;

	float r = n * tan(fovx*M_PI/360.);
	float l = -r;
	
	float t = n * tan(fovy*M_PI/360.);
	float b = -t;
	
	matrix4x4f *m = &projectionMatrix;
	m->m[0]=2*n/(r-l);	m->m[4]=0;			m->m[8]=(r+l)/(r-l);	m->m[12]=0;
    m->m[1]=0;			m->m[5]=2*n/(t-b);	m->m[9]=(t+b)/(t-b);	m->m[13]=0;
    m->m[2]=0;			m->m[6]=0;			m->m[10]=-(f+n)/(f-n);	m->m[14]=-(2*f*n)/(f-n);
    m->m[3]=0; 			m->m[7]=0; 			m->m[11]=-1;			m->m[15]=0;	
}

void updateViewportMatrix( double lv, double rv, double bv, double tv )
{
	matrix4x4f *m = &viewportMatrix;
	m->m[0]=(rv-lv)/2;	m->m[4]=0;			m->m[8]=0;	m->m[12]=(rv+lv)/2;
    m->m[1]=0;			m->m[5]=(tv-bv)/2;	m->m[9]=0;	m->m[13]=(tv+bv)/2;
    m->m[2]=0;			m->m[6]=0;			m->m[10]=1;	m->m[14]=0;
    m->m[3]=0; 			m->m[7]=0; 			m->m[11]=0;	m->m[15]=1;
}

void reshape2(int w, int h) {
	
	glutSetWindow(ninjaWindow);
	
	width = w;
	height = h;
	
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0., 1., 0., 1.);
	
	glMatrixMode(GL_MODELVIEW);
	
	updateProjectionMatrix();
	
	updateViewportMatrix(0., 1., 0., 1.);
}

void drawTriangles2d( vector<Triangle4f> tris )
{
	bool isColored = enableColoredDraw;

	switch(drawOpt) {
		case 0:	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL ); break;
		case 1:	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ); break;
		case 2:	glPolygonMode( GL_FRONT_AND_BACK, GL_POINT ); break;
	}

	glBegin(GL_TRIANGLES);
	if(isColored)
		glColor3f( forceColor.x,forceColor.y,forceColor.z );
	for(unsigned int i=0; i<tris.size(); i++) {
		for(int k=0; k<3; k++) {
			if(!isColored)
				glColor3f( tris[i].v[k].color[0],
							tris[i].v[k].color[1],
							tris[i].v[k].color[2]);
			glVertex2f(tris[i].v[k].pos.x, tris[i].v[k].pos.y);
		}
	}
	glEnd();
}

void display2 () {
// The NinjaGL Display Function
	
	glutSetWindow(ninjaWindow);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	// updates the Model/View Matrix with actual camera settings
	updateModelviewMatrix();
		
	// compute P*M
	matrix4x4f pm = projectionMatrix * modelviewMatrix;
	
	/*cout << "M:" << endl;
	modelviewMatrix.print();
	cout << "P:" << endl;
	projectionMatrix.print();
	cout << "PM:" << endl;
	pm.print();*/
	
		vector<Triangle4f> tris = objTris;
		
		// Projects from WCS to View Volume
		for(unsigned int i=0; i<tris.size(); i++)
			for(unsigned int vi=0; vi<3; vi++) {
				//pm.transformVector( &tris[i].v[vi].pos );
				modelviewMatrix.transformVector( &tris[i].v[vi].pos );
				projectionMatrix.transformVector( &tris[i].v[vi].pos );
				
				//printf("v%i:\t%.3f\t%.3f\t%.3f\t%.3f\n",vi, tris[i].v[vi].pos.x,tris[i].v[vi].pos.y,tris[i].v[vi].pos.z-cameraPos.z,tris[i].v[vi].pos.w);
			}

		// Do clipping
		vector<Triangle4f> tempTris;
		for(unsigned int i=0; i<tris.size(); i++) {
			
				double w1 = abs(tris[i].v[0].pos.w);
				double w2 = abs(tris[i].v[1].pos.w);
				double w3 = abs(tris[i].v[2].pos.w);
				
				if( abs(tris[i].v[0].pos.x) <= w1 &&
					abs(tris[i].v[0].pos.y) <= w1 &&
					abs(tris[i].v[0].pos.z) <= w1 &&
					
					abs(tris[i].v[1].pos.x) <= w2 &&
					abs(tris[i].v[1].pos.y) <= w2 &&
					abs(tris[i].v[1].pos.z) <= w2 &&
					
					abs(tris[i].v[2].pos.x) <= w3 &&
					abs(tris[i].v[2].pos.y) <= w3 &&
					abs(tris[i].v[2].pos.z) <= w3    ) {
					
					tempTris.push_back(tris[i]);
				}
		}
		
		// Do culling
		if(enableCulling) {
			tris.clear();
			for(unsigned int i=0; i<tempTris.size(); i++) {
				
				vector3f tnormal = vector3f(tempTris[i].normal.x,tempTris[i].normal.y,tempTris[i].normal.z);
				
				switch(orientationOpt) {
					case 0:
						if( dotProduct(-cameraN,tnormal) > 0 )
							tris.push_back(tempTris[i]);
						break;
					case 1:
						if( dotProduct(-cameraN,tnormal) < 0 )
							tris.push_back(tempTris[i]);
						break;
				}
			}
		}
		else
			tris = tempTris;
		

		for(unsigned int i=0; i<tris.size(); i++)
			for(unsigned int vi=0; vi<3; vi++) {
				// Perspective division				
				tris[i].v[vi].pos = tris[i].v[vi].pos / tris[i].v[vi].pos.w;

				// Map vertices to the Window
				viewportMatrix.transformVector( &tris[i].v[vi].pos );
			}
			
		// Do the 2D drawing
		drawTriangles2d(tris);
		drawOsd();
		
	glutSwapBuffers();
	glutPostRedisplay();
	frameCounter2++;
}


// PROGRAM /////////////////////////////////////////////////////////////

void initWorld()
{
	sprintf(fileName,"data/cow.in");
	//sprintf(fileName,"data/cube.in");
	loadModel();
}

void loadModel( int nil )
{
	object = Object(fileName);

	objTris.resize( object.tris.size() );
	for(unsigned int i=0; i<object.tris.size(); i++)
		for(unsigned int vi=0; vi<3; vi++) {
			objTris[i].v[vi].pos.x = object.tris[i].v[vi].pos.x;
			objTris[i].v[vi].pos.y = object.tris[i].v[vi].pos.y;
			objTris[i].v[vi].pos.z = object.tris[i].v[vi].pos.z;
			objTris[i].v[vi].pos.w = 1;
			
			objTris[i].v[vi].normal.x = object.tris[i].v[vi].normal.x;
			objTris[i].v[vi].normal.y = object.tris[i].v[vi].normal.y;
			objTris[i].v[vi].normal.z = object.tris[i].v[vi].normal.z;
			objTris[i].v[vi].normal.w = 1;
			
			objTris[i].v[vi].color[0] = object.tris[i].v[vi].color[0];
			objTris[i].v[vi].color[1] = object.tris[i].v[vi].color[1];
			objTris[i].v[vi].color[2] = object.tris[i].v[vi].color[2];
			
			objTris[i].normal.x = object.tris[i].normal.x;
			objTris[i].normal.y = object.tris[i].normal.y;
			objTris[i].normal.z = object.tris[i].normal.z;
			objTris[i].normal.w = 1;
		}
	
	resetCamera();
}

void initLights () {
	
	// ambient light
	//GLfloat ambientColor[] = {0.1, 0.1, 0.1, 1.0f};
	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
	
	{
		// fixed light
		GLfloat ambientLight[] = { 0.0, 0.0, 0.0, 1.0f };
		GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8f, 1.0f };
		glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	}
	
	{
		// rotating light
		GLfloat ambientLight[] = { 0.0, 0.0, 0.0, 1.0f };
		GLfloat diffuseLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
		GLfloat specularLight[] = { 0.8f, 0.8f, 0.8f, 1.0f };
		glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLight);
		glLightfv(GL_LIGHT1, GL_SPECULAR, specularLight);
		glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.2);
		glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.001);
	}
}

void initGL2()
{
    glClearColor(BG_COLOR, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glEnable(GL_NORMALIZE);		//normalizes all normals
    //glEnable(GL_COLOR_MATERIAL);
    //glEnable(GL_DEPTH_TEST);

	//updateSettings();
}

void initGL()
{
    glClearColor(BG_COLOR, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glEnable(GL_NORMALIZE);		//normalizes all normals
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_TEST);

	updateSettings();
}

void keyboardFunc (unsigned char key, int x, int y) {
			
    if ( key=='r' )
		resetCamera();
	
    if ( key==K_ESC )
	    exit(0);
}

void specialFunc(int key, int x, int y)
{
	heldCtrl = (glutGetModifiers() == GLUT_ACTIVE_CTRL);
	heldShift = (glutGetModifiers() == GLUT_ACTIVE_SHIFT);
	int var = 10;
	if(heldShift)
		var = 3;
		
	if( key == GLUT_KEY_LEFT || key == GLUT_KEY_DOWN )
		var = -var;
	
	if( key == GLUT_KEY_RIGHT || key == GLUT_KEY_LEFT )
	{
		cameraPos.x += var*cameraU.x;
		cameraPos.y += var*cameraU.y;
		cameraPos.z += var*cameraU.z;
		
		if(cameraMoveOpt==1) {
			float d = sqrt( pow(object.centerPoint.x-cameraPos.x,2) + pow(object.centerPoint.z-cameraPos.z,2) );
			float theta = asin(var/d);
			theta = theta*180/M_PI;
			
			matrix4x4f rot;
			rot.rotate(theta,cameraV);
			rot.transformVector(&cameraU);
			rot.transformVector(&cameraN);
		}
	}
	else if( key == GLUT_KEY_DOWN || key == GLUT_KEY_UP ) {
		cameraPos.x += var*cameraV.x;
		cameraPos.y += var*cameraV.y;
		cameraPos.z += var*cameraV.z;
		
		if(cameraMoveOpt==1) {
			float d = sqrt( pow(object.centerPoint.y-cameraPos.y,2) + pow(object.centerPoint.z-cameraPos.z,2) );
			float theta = asin(var/d);
			theta = theta*180/M_PI;
			
			matrix4x4f rot;
			rot.rotate(-theta,cameraU);
			rot.transformVector(&cameraV);
			rot.transformVector(&cameraN);
		}
	}
	
}

void mouseFunc(int button, int state, int x, int y) {
 
	heldCtrl = (glutGetModifiers() == GLUT_ACTIVE_CTRL);
	heldShift = (glutGetModifiers() == GLUT_ACTIVE_SHIFT);
 	
	int var = heldShift ? 3 : 10;
		
	if( button == GLUT_LEFT_BUTTON )
		left_click = state;
	if( button == GLUT_RIGHT_BUTTON )
		right_click = state;
	if( button == GLUT_WHEEL_MIDDLE )
		middle_click = state;
	if( button == GLUT_WHEEL_DOWN ) {
		var = -var;
	}
	if( button == GLUT_WHEEL_DOWN || button == GLUT_WHEEL_UP ) {
		cameraPos.x += var*cameraN.x;
		cameraPos.y += var*cameraN.y;
		cameraPos.z += var*cameraN.z;
	}
		
	xold = x;
	yold = y;
}

void mouseMotionFunc(int x, int y) {
	
	double sf;
	sf = heldShift ? 20. : 5.;
	
	float varX = -(x-xold)/5.;
	float varY = -(y-yold)/5.;
	
	if( left_click == GLUT_DOWN && right_click == GLUT_DOWN ) {
	
	}
    else if ( left_click == GLUT_DOWN ) {
		
		matrix4x4f rot;
		
		rot.rotate(varX,cameraV);
		rot.transformVector(&cameraU);
		rot.transformVector(&cameraN);

		rot.rotate(varY,cameraU);
		rot.transformVector(&cameraV);
		rot.transformVector(&cameraN);
    }
    else if ( right_click == GLUT_DOWN ) {
		
		matrix4x4f rot;
		
		rot.rotate(varX,cameraN);
		rot.transformVector(&cameraV);
		rot.transformVector(&cameraU);
	}
	
	if ( middle_click ==GLUT_DOWN  ) {
		
	}

	xold = x;
	yold = y;
}

void updateFPS(int value) {
	fps = frameCounter;
	frameCounter = 0;
	
	sprintf(osd[0],"FPS: %li ",fps);
	
	glutTimerFunc(1000/*1sec*/, updateFPS, 0);
}

void updateFPS2(int value) {
	fps2 = frameCounter2;
	frameCounter2 = 0;
	
	sprintf(osd2[0],"FPS: %li ",fps2);
	
	glutTimerFunc(1000/*1sec*/, updateFPS2, 0);
}

void resetCamera( int nil )
{
	cameraU={1,0,0};
	cameraV={0,1,0};
	cameraN={0,0,-1};
	
	cameraPos.x = object.centerPoint.x*object.size[0] + object.pos[0];
	cameraPos.y = object.centerPoint.y*object.size[1] + object.pos[1];	
	
	double z, z1, z2;
	
	//get the optimal z acording to Y dimensions (height) of the object
	z1 = (object.maxPoint.y-object.minPoint.y) / (2.*tan(fovy*M_PI/360.));
	
	//get the optimal z acording to X dimensions (width) of the object	
	z2 = (object.maxPoint.x-object.minPoint.x) / (2.*tan(fovx*M_PI/360.));
	
	//decide which z to take
	z = z1>z2 ? z1 : z2;
	
	cameraPos.z = z + object.maxPoint.z*object.size[2] + object.pos[2];
}	

void callReshape( int nil )
{
	reshape(width,height);
	reshape2(width,height);
}

void updateSettings( int nil )
{
	glutSetWindow(mainWindow);
	
	// Light
	enableLight ? glEnable(GL_LIGHTING) : glDisable(GL_LIGHTING);
	enableFixedLight ? glEnable(GL_LIGHT0) : glDisable(GL_LIGHT0);
	enableRotLight ? glEnable(GL_LIGHT1) : glDisable(GL_LIGHT1);
	
	// OpenGL
	switch(shadeOpt) {
		case 0: glShadeModel(GL_SMOOTH); break;
		case 1: glShadeModel(GL_FLAT); break;
	}		
	enableCulling ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
	switch(orientationOpt) {
		case 0: glFrontFace(GL_CW); break;
		case 1: glFrontFace(GL_CCW); break;
	}
}

void createGuiWindow()
{
	GLUI *glui = GLUI_Master.create_glui("GUI",0,WINDOWSX+width+10,WINDOWSY);

	GLUI_Panel *lp = glui->add_panel("World");
		glui->add_checkbox_to_panel( lp, "Enable Lighting", &enableLight, 0, updateSettings );
		glui->add_checkbox_to_panel( lp, "Enable LIGHT0", &enableFixedLight, 0, updateSettings );
		glui->add_checkbox_to_panel( lp, "Enable LIGHT1", &enableRotLight, 0, updateSettings );
	
	GLUI_Panel *mp = glui->add_panel("Model");
		GLUI_Panel *mpm = glui->add_panel_to_panel(mp,"");
			glui->add_edittext_to_panel( mpm, "File name:", GLUI_EDITTEXT_TEXT, fileName );
			glui->add_button_to_panel( mpm, "Load", 0, loadModel );
		GLUI_Listbox *orientations = glui->add_listbox_to_panel(mp, "V. Orientation: ", &orientationOpt, 0, updateSettings);
			orientations->add_item( 0, "CW");
			orientations->add_item( 1, "CCW");
		GLUI_Listbox *shadings = glui->add_listbox_to_panel(mp, "Shading: ", &shadeOpt, 0, updateSettings);
			shadings->add_item( 0, "Smooth");
			shadings->add_item( 1, "Flat");
		GLUI_Listbox *models = glui->add_listbox_to_panel(mp,"Drawing Mode: ", &drawOpt, 0, updateSettings);
			models->add_item( 0, "Filled");
			models->add_item( 1, "Wireframe");
			models->add_item( 2, "Vertex");
		glui->add_checkbox_to_panel( mp, "Enable Backface Culling", &enableCulling, 0, updateSettings );
		glui->add_checkbox_to_panel( mp, "Draw Normals", &enableDrawNormals );
		glui->add_checkbox_to_panel( mp, "Draw Bounding Box", &enableDrawBoundingBox );
			GLUI_Panel *mpc = glui->add_panel_to_panel(mp,"Coloring");
				glui->add_checkbox_to_panel( mpc, "Enable", &enableColoredDraw );
				GLUI_Spinner *rSpin = glui->add_spinner_to_panel( mpc, "R:", GLUI_SPINNER_FLOAT, &forceColor.x );
					rSpin->set_float_limits( 0., 1., GLUI_LIMIT_CLAMP );
				GLUI_Spinner *gSpin = glui->add_spinner_to_panel( mpc, "G:", GLUI_SPINNER_FLOAT, &forceColor.y );
					gSpin->set_float_limits( 0., 1., GLUI_LIMIT_CLAMP );
				GLUI_Spinner *bSpin = glui->add_spinner_to_panel( mpc, "B:", GLUI_SPINNER_FLOAT, &forceColor.z );
					bSpin->set_float_limits( 0., 1., GLUI_LIMIT_CLAMP );
	
	//glui->add_column(false);

	GLUI_Panel *cp = glui->add_panel("Camera");
		GLUI_Panel *cpm = glui->add_panel_to_panel(cp, "Movement");
			GLUI_RadioGroup *cameragroup = glui->add_radiogroup_to_panel(cpm, &cameraMoveOpt);
				glui->add_radiobutton_to_group( cameragroup, "Free" );
				glui->add_radiobutton_to_group( cameragroup, "Centering" );
		glui->add_spinner_to_panel( cp, "fovx:", GLUI_SPINNER_FLOAT, &fovx, 0, callReshape );
		glui->add_spinner_to_panel( cp, "fovy:", GLUI_SPINNER_FLOAT, &fovy, 0, callReshape );
		glui->add_spinner_to_panel( cp, "Near Clip:", GLUI_SPINNER_FLOAT, &clipNear, 0, callReshape );
		glui->add_spinner_to_panel( cp, "Far Clip:", GLUI_SPINNER_FLOAT, &clipFar, 0, callReshape );
		glui->add_button_to_panel( cp, "Reset position", 0, resetCamera );

	glui->add_button( "EXIT", 0, exit );
	glui->set_main_gfx_window( mainWindow );
}


int main (int argc, char **argv) {
    cout << "Initializing...\n";
    
    // OpenGL Window
    glutInit (&argc, argv);
    glutInitDisplayMode ( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutInitWindowSize (width, height);
	glutInitWindowPosition(WINDOWSX,WINDOWSY);
	mainWindow = glutCreateWindow ("OpenGL");
	
    glutDisplayFunc (display);
    glutReshapeFunc(reshape);
    GLUI_Master.set_glutIdleFunc (display);
    
    glutKeyboardFunc(keyboardFunc);
    glutSpecialFunc(specialFunc);
    glutMouseFunc(mouseFunc);
	glutMotionFunc(mouseMotionFunc);
	
	initGL();
	initLights();
	glutTimerFunc(1000/*1sec*/, updateFPS, 0);
	
	// Close2GL Window
	glutInitWindowPosition(WINDOWSX+width+220,WINDOWSY);
	ninjaWindow = glutCreateWindow ("NinjaGL");
	
    glutDisplayFunc(display2);
    glutReshapeFunc(reshape2);
    GLUI_Master.set_glutIdleFunc(display2);
    
    glutKeyboardFunc(keyboardFunc);
    glutSpecialFunc(specialFunc);
    glutMouseFunc(mouseFunc);
	glutMotionFunc(mouseMotionFunc);
	
	initGL2();
	glutTimerFunc(1000/*1sec*/, updateFPS2, 0);
	
	// More inits
    initWorld();
	
	createGuiWindow();
	
    glutMainLoop();     
    
    return 0;
} 
