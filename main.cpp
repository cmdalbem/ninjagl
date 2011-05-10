#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <vector>
#include <string.h>

#include <GL/glui.h>
#include <GL/gl.h>
#include <GL/glut.h>

#include "matrix4x4f.h"
#include "constants.h"
#include "Object.h"
						 
using namespace std;


// GLOBALS ////////////////////////////////////////////////////////////

#define 		BG_COLOR 0.2, 0.2, 0.3
#define 		OSD_LINES 2
			
int				mainWindow;
Object			object;

// Mouse-Keyboard
static int      xold, yold;		
static int	    left_click = GLUT_UP;
static int	    right_click = GLUT_UP;
static int	    middle_click = GLUT_UP;
static bool     heldCtrl = false;
static bool     heldShift = false;

long int		frameCounter, fps;
char 			osd[OSD_LINES][256];
Point3df		cameraPos;
int				width=800, height=600;
vector3f		cameraU, cameraV, cameraN;

// GUI controlled
float			fovy=90, clipNear=0.1, clipFar=1000;
int				drawOpt, cameraMoveOpt, orientationOpt, shadeOpt;
int				enableFixedLight=1, enableRotLight=1, enableCulling=1, 
				enableDrawNormals=0, enableDrawBoundingBox=0, enableColoredDraw=0,
				enableLight=1;
Point3df		forceColor={1,1,1};
char			fileName[256];


// PROTOTYPES //////////////////////////////////////////////////////////

void resetCamera( int nil=0 );
void updateSettings( int nil=0 );
void loadModel( int nil=0 );

// PROGRAM /////////////////////////////////////////////////////////////

void drawObjects()
{
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

		glRasterPos3f(-1.3,0.9,-1);
		for( int ch = 0; ch < (int)strlen(osd[0]); ch++) 
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, osd[0][ch]);

		glRasterPos3f(-1.3,0.9-0.08,-1);
		for( int ch = 0; ch < (int)strlen(fileName); ch++) 
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, fileName[ch]);
   
	enableLight ? glEnable(GL_LIGHTING) : glDisable(GL_LIGHTING);
}

void reshape(int w, int h)
{
	width = w;
	height = h;
}

void display () {
	
	glutSetWindow(mainWindow);
	
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	glViewport(0, 0, (GLsizei) width, (GLsizei) height);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective (fovy, (GLfloat)width / (GLfloat)height, clipNear, clipFar);
	  
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity ();
    
		drawOsd();
		camera();
		lights();
		drawObjects();
	
	glutSwapBuffers();

	frameCounter++;
	glutPostRedisplay();
}

void initWorld()
{
	sprintf(fileName,"data/cow.in");
	loadModel();
}

void loadModel( int nil )
{
	object = Object(fileName);
	resetCamera();
}

void initLights () {
	
	// ambient light
	GLfloat ambientColor[] = {0.1, 0.1, 0.1, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
	
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

void initGL()
{
    glClearColor(BG_COLOR, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glEnable(GL_NORMALIZE);		//normalizes all normals
	glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);

	updateSettings();

	initLights();
}

//--------------------------- KEYBOARD ---------------------------//
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

//--------------------------- MOUSE ---------------------------//
void mouseFunc(int button, int state, int x, int y) {
/* This function only updates click states and positions */
 
	heldCtrl = (glutGetModifiers() == GLUT_ACTIVE_CTRL);
	heldShift = (glutGetModifiers() == GLUT_ACTIVE_SHIFT);
 	
 	int var = 10;
	if(heldShift)
		var = 3;
		
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
	if(heldShift)
		sf = 20.;
	else
		sf = 5.;
	
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

void resetCamera( int nil )
{
	cameraU={1,0,0};
	cameraV={0,1,0};
	cameraN={0,0,-1};
	
	cameraPos.x = object.centerPoint.x*object.size[0] + object.pos[0];
	cameraPos.y = object.centerPoint.y*object.size[1] + object.pos[1];	
	
	double z, z1, z2;
	
	z1 = ( (object.maxPoint.y-object.minPoint.y)*object.size[1] + object.pos[1])
			/ 2.*tan(fovy*M_PI/360.);
	
	double fovx = 2*atan( tan(fovy*M_PI/360.)*height/(double)width );
	
	z2 = ( (object.maxPoint.x-object.minPoint.x)*object.size[0] + object.pos[0])
			/ 2.*tan(fovx/2.);
	
	z = z1>z2 ? z1 : z2;
	
	cameraPos.z = z + object.maxPoint.z*object.size[2] + object.pos[2];	
}	

void updateSettings( int nil )
{
	// Light
	enableLight ? glEnable(GL_LIGHTING) : glDisable(GL_LIGHTING);
	enableFixedLight ? glEnable(GL_LIGHT0) : glDisable(GL_LIGHT0);
	enableRotLight ? glEnable(GL_LIGHT1) : glDisable(GL_LIGHT1);
	
	// Model Drawing
	switch(drawOpt) {
		case 0:	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL ); break;
		case 1:	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ); break;
		case 2:	glPolygonMode( GL_FRONT_AND_BACK, GL_POINT ); break;
	}
	
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
	GLUI *glui = GLUI_Master.create_glui("GUI");

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
		glui->add_spinner_to_panel( cp, "Field of view:", GLUI_SPINNER_FLOAT, &fovy );
		glui->add_spinner_to_panel( cp, "Near Clip:", GLUI_SPINNER_FLOAT, &clipNear );
		glui->add_spinner_to_panel( cp, "Far Clip:", GLUI_SPINNER_FLOAT, &clipFar );
		glui->add_button_to_panel( cp, "Reset position", 0, resetCamera );

	glui->add_button( "EXIT", 0, exit );
	glui->set_main_gfx_window( mainWindow );
}

int main (int argc, char **argv) {
    cout << "Initializing...\n";
    glutInit (&argc, argv);
    glutInitDisplayMode ( GLUT_DOUBLE | GLUT_DEPTH ); //set the display to Double buffer, with depth buffer
    
	glutInitWindowSize (width, height);
	mainWindow = glutCreateWindow ("CloseToGL");
	createGuiWindow();
	
    initGL();
    initWorld();
    
    glutDisplayFunc (display);
    glutReshapeFunc(reshape);
    GLUI_Master.set_glutIdleFunc (display);
    glutTimerFunc(1000/*1sec*/, updateFPS, 0);
    
    glutKeyboardFunc(keyboardFunc);
    glutSpecialFunc(specialFunc);
    glutMouseFunc(mouseFunc);
	glutMotionFunc(mouseMotionFunc);
	
    glutMainLoop();     
    
    return 0;
} 
