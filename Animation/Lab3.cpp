#include <windows.h>	// for timeGetTime()
#include <mmsystem.h>	// ditto
#include <iostream>		// I/O
#include <GL/freeglut.h>
#include <glm.hpp>		// Mathematic library

#include <math.h>
#include "textureTGA.h"
#include "SkyBox.h"
#include "Camera.h"

void setupScene();
void updateScene();
void renderScene();
void exitScene();
void keyPressed(unsigned char key, int x, int y);
void keySpecialPressed(unsigned char key, int x, int y);
void setViewport(int width, int height);
void setOrthogonalView();
void disableOrthogonalView();
void drawTarget();
void computeInverseKinematicsAngles();

bool		wireframe=false;
int         windowId;
DWORD		lastTickCount;
GLfloat		windowHeight(600), windowWidth(800);

GLfloat white_light[] = {1.0, 1.0, 1.0, 1.0};
GLfloat left_light_position[] = {1,0,-1, 1.0}; 
GLfloat right_light_position[] = {-1,0,-1, 1.0};

GLfloat grey_ambient[] = {0.01745, 0.01745, 0.01745};
GLfloat grey_diffuse[] = {0.161424, 0.161424, 0.161424};
GLfloat grey_specular[] = {0.727811, 0.727811, 0.727811};
GLfloat grey_shininess = 76.8;

GLfloat BoneA_Length = 1.0;
GLfloat BoneB_Length = 0.7;
GLfloat BoneA_Angle(0), BoneB_Angle(0);

glm::vec2 target2D_position(BoneA_Length+BoneB_Length, 0);
GLfloat deltaPos = 0.01;
bool invertedSolution = false;

SkyBox * skybox;

Camera * camera;


void renderScene(){
        
    // Clear framebuffer & depth buffer
	glClearColor(0.5,0.8,0.7,0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//Enable lighting
 	glEnable(GL_LIGHTING);

	//Set the material properties
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, grey_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, grey_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, grey_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, grey_shininess);   
	skybox->draw();
	// Reset Modelview matrix      	
    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	


	glTranslatef(0, -2, 0);
	glutSolidCube(1.0f);
	
	camera->draw();
	

	

	glDisable(GL_LIGHTING);
	
    // Swap double buffer for flicker-free animation
    glutSwapBuffers();
        
}

void drawTarget()
{
	// Draw the targer
	glPushMatrix(); //remember the current state of the modelview matrix
	
	glTranslatef(0,target2D_position[1],target2D_position[0]);

	glColor3ub(255, 0, 0);
	glLineWidth(4.0);
	glBegin(GL_LINES);
	glVertex3f(0.0, -0.1, 0);
	glVertex3f(0.0,  0.1, 0);
	glVertex3f(0.0, 0, -0.1);
	glVertex3f(0.0, 0,  0.1);
	glEnd();

	glPopMatrix(); //restore the state of the modelview matrix
}



/*GLfloat BoneA_Length = 1.0;
GLfloat BoneB_Length = 0.7;
GLfloat BoneA_Angle(0), BoneB_Angle(0);

glm::vec2 target2D_position(X, Y);*/




void updateScene(){
	
	// Wait until at least 16ms passed since start of last frame
	// Effectively caps framerate at ~60fps
	while(timeGetTime()-lastTickCount<16);
	lastTickCount=timeGetTime();

	//computeInverseKinematicsAngles();
    camera->move_player();
    // Do any other updates here
	
	// Draw the next frame
    glutPostRedisplay();

}

void keyPressed(unsigned char key, int x, int y){
	
	switch (key)
	{
	case 27:
		// Test if user pressed ESCAPE (ascii 27)
		// If so, exit the program
		exitScene();
		break;
	default:
		break;
	}

	camera->keypressdown(key, x, y);
}

void keyPressedUp(unsigned char key, int x, int y)
{
	camera->keypressup(key, x , y);
}

void keySpecialPressed (int key, int x, int y) {  
	switch (key)
	{
	case GLUT_KEY_LEFT:
		target2D_position[0] -= deltaPos;
		break;
	case GLUT_KEY_RIGHT:
		target2D_position[0] += deltaPos;
		break;
	case GLUT_KEY_DOWN:
		target2D_position[1] -= deltaPos;
		break;
	case GLUT_KEY_UP:
		target2D_position[1] += deltaPos;
		break;

	default:
		break;
	}
}

void setupScene(){

	std::cout<<"Initializing scene..."<<std::endl;
    
	//Set up Lighting Stuff
	glLightfv(GL_LIGHT0, GL_POSITION, left_light_position);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
	glLightfv(GL_LIGHT1, GL_POSITION, right_light_position);
	glLightfv(GL_LIGHT1, GL_SPECULAR, white_light);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, white_light);
	glShadeModel(GL_SMOOTH);

	skybox = new SkyBox( TERRIAN_WIDTH, BORDER );
	camera = new Camera();

	glEnable(GL_DEPTH_TEST);        
}

void exitScene(){

    std::cout<<"Exiting scene..."<<std::endl;

    // Close window
    glutDestroyWindow(windowId);

    // Free any allocated memory

    // Exit program
    exit(0);
}

void setViewport(int width, int height) {

    // Work out window ratio, avoid divide-by-zero
    if(height==0)height=1;
	float ratio = float(width)/float(height);

	// Reset projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Fill screen with viewport
	glViewport(0, 0, width, height);

	// Set a 45 degree perspective
	gluPerspective(45, ratio, .1, 1000);

}

//Can't have callback to another class (or someting) so need this
void mouse_move(int x, int y)
{
	camera->mouse_move(x, y);
}




int main(int argc, char *argv[]){
        
    // Initialise OpenGL
    glutInit(&argc, argv); 

    // Set window position, size & create window
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(50,50);
    glutInitWindowSize(windowWidth,windowHeight);
	windowId = glutCreateWindow("IET Lab: 2D Inverse Kinematics");
    
    // Set GLUT callback functions
    glutReshapeFunc(setViewport);
    glutDisplayFunc(renderScene);
    glutIdleFunc(updateScene);
    glutKeyboardFunc(keyPressed);
	glutKeyboardUpFunc(keyPressedUp);
	glutSpecialFunc(keySpecialPressed); // Tell GLUT to use the method "keySpecialPressed" for special key presses
	glutMotionFunc(mouse_move);

    // Setup OpenGL state & scene resources (models, textures etc)
    setupScene();

    // Show window & start update loop
    glutMainLoop();    

    return 0;
    
}
