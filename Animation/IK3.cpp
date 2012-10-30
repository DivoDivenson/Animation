#include <windows.h>	// for timeGetTime()
#include <mmsystem.h>	// ditto
#include <iostream>		// I/O
#include <GL/freeglut.h>
#include <glm.hpp>		// Mathematic library

#define _USE_MATH_DEFINES //To include the constante M_PI
#define GL_PI 3.1415926535897932384626433832795
#include <math.h>
#include "consts.h"

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

GLfloat BoneA_Length = 0.65;
GLfloat BoneB_Length = 0.55;
GLfloat BoneC_Length = 0.55;
GLfloat BoneA_Angle(0), BoneB_Angle(0), BoneC_Angle(0);


glm::vec2 target2D_position(1.12f, -0.1f);

glm::vec3 leg_pos[] = {glm::vec3(1.12f, -0.1f, 0.0f), glm::vec3(0.94f, -0.1f, 0.0f), glm::vec3(0.94f, -0.1f, 0.0f), glm::vec3(1.12f, -0.1f, 0.0f),};

glm::vec3 leg_angles[] = {glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)};
GLfloat deltaPos = 0.01;
bool invertedSolution = false;

//Need code to figure out the required speed
glm::vec2 key_frames[] = { glm::vec2(1.12f, -0.1f), glm::vec2(0.94f, -0.1f), glm::vec2(1.03f, 0.2f) };
int frames[] = {0, 1, 1, 0}; //Each leg starts at a different "frame"



void draw_leg(float x, float y_rot, int leg_no)
{


	//To setup the creation of quadric objects
	GLUquadric* nQ;
	nQ=gluNewQuadric();

    glPushMatrix(); //remember the current state of the modelview matrix

	glTranslatef(x, 0, 0);
	glRotatef(y_rot, 0, 1, 0);
	// Rotate from the angle between the first bone and the Z axis
	glRotatef(leg_angles[leg_no][0],1, 0, 0);

	//Draw the first cone at the origin
	gluCylinder(nQ, 0.1, 0.01, BoneA_Length, 30, 5) ;
	gluSphere(nQ, 0.1, 30, 5);
	
	// Translate by the bone length
	glTranslatef(0,0.0,BoneA_Length);

	glPushMatrix(); //remember the current state of the modelview matrix

	// Rotate from the angle between the first and second bones
	glRotatef(leg_angles[leg_no][1],1,0,0);

	//Draw the second cone at the origin, pointing up
	gluCylinder(nQ, 0.1, 0.01, BoneB_Length, 30, 5) ;
	gluSphere(nQ, 0.1, 30, 5);

	
	glTranslatef(0, 0, BoneB_Length);

	glPushMatrix();

	glRotatef(leg_angles[leg_no][2],1,0,0);

	//Draw the second cone at the origin, pointing up
	gluCylinder(nQ, 0.1, 0.01, BoneC_Length, 30, 5) ;
	gluSphere(nQ, 0.1, 30, 5);


	glPopMatrix();


	glPopMatrix(); //restore the state of the modelview matrix
	
	glPopMatrix(); //restore the state of the modelview matrix


}
 
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

	// Reset Modelview matrix      	
    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	// Set view position & direction (Camera at (-5,0,0) looking on the positive X-axis). World is Y-up.
	gluLookAt(-5,0,0, 1,0,0, 0,1,0);
	glRotatef(90.0f, 0, 0, 1);
	//glRotatef(45.0f, -1, 0, 0);
	//glRotatef(45.0f, 0, -1, 0);

	glPushMatrix();
	draw_leg(0, -20.0f, 0);
	draw_leg(1.0f, 20.0f, 1);
	glPushMatrix();
		glRotatef(180.0f, 0, 1, 0);
		glTranslatef(0, 0, 1.0);
		draw_leg(-1.0f, -20.0f, 2);
		draw_leg(0, 20.0f, 3);
	glPopMatrix();
	glPopMatrix();
	//draw_leg(3.0f);
	
	//drawTarget();

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


void computeInverseKinematicsAngles(int leg_no)
{
	GLfloat d = glm::sqrt( glm::pow(leg_pos[leg_no][0], 2.0f) + glm::pow(leg_pos[leg_no][1], 2.0f));
	GLfloat theta;
	

	if( d >= ( BoneA_Length + BoneB_Length + BoneC_Length) )
	{
		d = BoneA_Length + BoneB_Length + BoneC_Length;
		//I'm sure there are much better ways to do this
		GLfloat slope = leg_pos[leg_no][1] / leg_pos[leg_no][0];
		GLfloat x = ( d * leg_pos[leg_no][0] ) / glm::sqrt( glm::pow(leg_pos[leg_no][0], 2.0f) + glm::pow(leg_pos[leg_no][1], 2.0f));
		theta = glm::acos( (x / d));
		//std::cout << "Slope: " << slope << " X: " << x << " Theta: " << theta << " D: " << d << "\n";
	}
	else
	{
		theta = glm::acos( (leg_pos[leg_no][0] / d));
	}



	
	leg_angles[leg_no][0] = ( glm::pow(d, 2.0f) + glm::pow(BoneA_Length, 2.0f) - glm::pow(BoneB_Length, 2.0f));
	leg_angles[leg_no][0] = leg_angles[leg_no][0] / (2.0f * BoneA_Length * d);
	//Prevent overlap
	if(leg_angles[leg_no][0] > 1.0f)
	{
		leg_angles[leg_no][0] = 1.0f;
	}
	leg_angles[leg_no][0] = acos(leg_angles[leg_no][0]);
	//No "negative" angle
	if(leg_pos[leg_no][1] >= 0 && !invertedSolution)
	{
		theta *= -1.0f;
	}
	else if(leg_pos[leg_no][1] <= 0 && invertedSolution)
	{
		theta *= -1.0f;
	}
	
	leg_angles[leg_no][0] -= theta;
	

	leg_angles[leg_no][1] = ( glm::pow(BoneA_Length, 2.0f) + glm::pow(BoneB_Length, 2.0f) - glm::pow(d, 2.0f));
	leg_angles[leg_no][1] = leg_angles[leg_no][1] / (2.0f * BoneA_Length * BoneB_Length);
	if(leg_angles[leg_no][1] <= -1.0f)
	{
		leg_angles[leg_no][1] = 0.0; //HACK, acos(-1) returns NaN for some reason. Yes, it was very annoying to discover.
	}
	else
	{
		if(leg_angles[leg_no][1] > 1.0f)
		{
			leg_angles[leg_no][1] = 1.0f;
		}
		leg_angles[leg_no][1] = glm::acos(leg_angles[leg_no][1]) - GL_PI; //Forget the logic behind the PI bit
	}

	
	
	// TODO: compute bone angles depending on the target position target2D_position
	
	// Convert the angles in degrees as mathematical operations are usually conducted in radians

	if(!invertedSolution)
	{
		leg_angles[leg_no][0] = glm::degrees(leg_angles[leg_no][0]) * -1.0f; //Invert the result because of some reason I can't quite put to words,
		leg_angles[leg_no][1] = glm::degrees(leg_angles[leg_no][1]) * -1.0f; //other than "OpenGL is the wrong way around"
	}
	else
	{
		leg_angles[leg_no][0] = glm::degrees(leg_angles[leg_no][0]);
		leg_angles[leg_no][1] = glm::degrees(leg_angles[leg_no][1]);
	}

	leg_angles[leg_no][2] = leg_angles[leg_no][1];

	//std::cout << "X: " << target2D_position[0] << " Y: " << target2D_position[1] << " D: " << d << "\n";
	//std::cout << "A: " << BoneA_Angle << " B: " << BoneB_Angle << " Theta: " << theta  << "\n\n";
	
}


int frame_inc(int frame, int direction)
{
	if(!direction)
	{
		if(frame >= 2 ) //Hardcode for the minute
		{
			frame = 0;
		}
		else
		{
			frame++;
		}
	}
	else
	{
		if(frame <= 0 ) //Hardcode for the minute
		{
			frame = 2;
		}
		else
		{
			frame--;
		}
	}
	

	return frame;
}

float move_toward(float pos, float target, float speed)
{

	//std::cout << "HELLO " << pos << " : " << target << " : " << pos / target <<"\n";
	if(abs(pos - target) < 0.00001) //Equality
	{
		//std::cout << "WORKS" << "\n";
		return pos;
	}
	else if(pos < target)
	{
		return pos += deltaPos * speed;
	}
	else if(pos > target)
	{
		return pos -= deltaPos * speed;
	}

}

void move_target(int leg_no, int direction)
{
	//If statement is a hack
	//std::cout << leg_no << " : " << frames[leg_no] << "\n";
	if( (abs(leg_pos[leg_no][0] - key_frames[frames[leg_no]][0]) >= EPSILON))// && (abs(target2D_position[1] - key_frames[frame][1]) >= EPSILON))
	{
		leg_pos[leg_no][0] = move_toward(leg_pos[leg_no][0], key_frames[frames[leg_no]][0], 1.0f);
		leg_pos[leg_no][1] = move_toward(leg_pos[leg_no][1], key_frames[frames[leg_no]][1], 3.0f);
	}
	else
	{
		//std::cout << "INC" << "\n";
		frames[leg_no] = frame_inc(frames[leg_no], direction);
	}
	//target2D_position[1] += deltaPos;
	//std::cout << "\n";
}


void updateScene(){
	
	// Wait until at least 16ms passed since start of last frame
	// Effectively caps framerate at ~60fps
	while(timeGetTime()-lastTickCount<16);
	lastTickCount=timeGetTime();

	computeInverseKinematicsAngles(0);
	move_target(0, 0);
	computeInverseKinematicsAngles(1);
	move_target(1, 0);
	computeInverseKinematicsAngles(2);
	move_target(2, 1);
	computeInverseKinematicsAngles(3);
	move_target(3, 1);

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

	case 'w':
	case 'W':
		wireframe=!wireframe;
		glPolygonMode(GL_FRONT_AND_BACK, (wireframe ? GL_LINE : GL_FILL));
		break;

	case 'i':
	case 'I':
		invertedSolution=!invertedSolution;
		break;

	case 'z':
	case 'Z':
		BoneA_Length -= deltaPos;
		break;

	case 'a':
	case 'A':
		BoneA_Length += deltaPos;
		break;

	case 'x':
	case 'X':
		BoneB_Length -= deltaPos;
		BoneC_Length -= deltaPos;
		break;

	case 's':
	case 'S':
		BoneB_Length += deltaPos;
		BoneC_Length += deltaPos;
		break;

	// Other possible keypresses go here
	/*case 'a':
		...
		break;*/

	default:
		break;
	}
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
	glutSpecialFunc(keySpecialPressed); // Tell GLUT to use the method "keySpecialPressed" for special key presses  
    // Setup OpenGL state & scene resources (models, textures etc)
    setupScene();

    // Show window & start update loop
    glutMainLoop();    

    return 0;
    
}
