//#include <windows.h>  // for timeGetTime()
//#include <mmsystem.h> // ditto
#include <iostream>     // I/O
//#include <glut.h>     // for gluPerspective & gluLookAt
#include <GL/freeglut.h>
#include <sys/time.h>

#include "Camera.h"

void setupScene();
void updateScene();
void renderScene();
void exitScene();
void keypress(unsigned char key, int x, int y);
void setViewport(int width, int height);

typedef unsigned int DWORD;

Camera * camera;

int         rotationAngle=0;
bool        wireframe=false;
int         windowId;
DWORD       lastTickCount;
GLfloat white_light[] = {1.0, 1.0, 1.0, 1.0};
GLfloat left_light_position[] = {1,0,-1, 1.0};
GLfloat right_light_position[] = {-1,0,-1, 1.0};

GLfloat emerald_ambient[] =
{0.0215, 0.1745, 0.0215}, emerald_diffuse[] =
{0.07568, 0.61424, 0.07568}, emerald_specular[] =
{0.633, 0.727811, 0.633}, emerald_shininess = 76.8;



void renderLeg(float x){


	//To setup the creation of quadric objects
    GLUquadric* nQ;
    nQ=gluNewQuadric();
    GLUquadric * nQ2;
    nQ2 = gluNewQuadric();

	glTranslatef(0.0f, 0.0f, 0.5f);
	glTranslatef(x, 0, 0);
	//Rotatae everything above the base

    glPushMatrix();
	glRotatef(30, 0, -1, 0);
	glRotatef(30, -1, 0, 0);
	gluCylinder(nQ, 0.15, 0.15, 1, 20, 5);

	glTranslatef(0, 0, 1);
	glTranslatef(0, -1, 0);

	glRotatef(90.f, -1, 0, 0);
	gluCylinder(nQ2, 0.15, 0.15, 1, 20, 5);
	glPopMatrix();



}

void renderScene(){

    // Clear framebuffer & depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Enable lighting
    glEnable(GL_LIGHTING);

    //Set the material properties
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, emerald_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, emerald_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, emerald_specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, emerald_shininess);

    // Reset Modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    camera->draw();
    glPushMatrix();
    //Go back 10 along Z axis so we can see the models
    gluLookAt(0,0,10,  0,0,-1,  0,1,0);
    //Rotate so things look isometric
    glRotatef(45.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(45.0f, 0.0f, -1.0f, 0.0f);

    //at origin draw base
    glutSolidCube(1.0f);

	renderLeg(-1);
	renderLeg(1);
	renderLeg(2);

    glPopMatrix();


/*    glTranslatef(0.0f, 0.5f, 0.0f);
    //rotate everything above the base
    glRotatef(-1*rotationAngle/4.f,0,1,0);
    glRotatef(-1*rotationAngle/3.f,1,0,0);
    glRotatef(-1*rotationAngle/2.f,0,0,1);
//  glPushMatrix();
    //draw first joint
    glutSolidSphere(0.3f, 20, 20);

    //cylinder, draw upper arm
    glPushMatrix();
    glRotatef(90.0f, -1.0f, 0.0f, 0.0f);
    gluCylinder(nQ, 0.15, 0.15, 1.5f, 20, 5);
    //glRotatef(90.0f, 1.0f, 0.0f, 0.0f); //Same as pop
    glPopMatrix(); //Undo the rotate

    glTranslatef(0.0f, 1.7f, 0.0f);
    //Rotate the lower arm
    glRotatef(-1*rotationAngle/4.f,0,1,0);
    glRotatef(-1*rotationAngle/3.f,1,0,0);
    glRotatef(-1*rotationAngle/2.f,0,0,1);

    //second join
    glutSolidSphere(0.3f, 20, 20);
    //Horizontal cylinder for lower arm
    gluCylinder(nQ2, 0.15, 0.15, 1.5f, 20, 5);

    glPopMatrix(); //restore the state of the modelview matrix
    // Swap double buffer for flicker-free animation
	*/
	//camera->draw();
    glutSwapBuffers();

}

void updateScene(){
    timeval tim;
    gettimeofday(&tim,NULL);
    // Wait until at least 16ms passed since    //To setup the creation of quadric objects

    // Effectively caps framerate at ~60fps
    double t2 = tim.tv_sec +(tim.tv_usec/1000000.0);
    while(t2-lastTickCount<16);
    lastTickCount=gettimeofday(&tim,NULL);

    // Increment angle for next frame
    rotationAngle+=2;

    // Do any other updates here
	camera->move_player();

    // Draw the next frame
    glutPostRedisplay();

}

void keypress(unsigned char key, int x, int y){

    // Test if user pressed ESCAPE (ascii 27)
    // If so, exit the program
    if(key==27){
        exitScene();
    }

    // 'W' key toggles wireframe mode on & off
    if(key == 'w' || key == 'W'){
        wireframe=!wireframe;
        if(wireframe){
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    // Other possible keypresses go here
    //if(key == 'a'){...}
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


void mouse_move(int x, int y){
	camera->mouse_move(x, y);
}


int main(int argc, char *argv[]){

    // Initialise OpenGL
    glutInit(&argc, argv);

    // Set window position, size & create window
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(50,50);
    glutInitWindowSize(640,480);
    windowId = glutCreateWindow("Graphics Lab 2: Hierarchical Transformations");

    camera = new Camera();
    // Setup OpenGL state & scene resources (models, textures etc)
    setupScene();

    // Set GLUT callback functions
    glutReshapeFunc(setViewport);
    glutDisplayFunc(renderScene);
    glutIdleFunc(updateScene);
    glutKeyboardFunc(keypress);
    glutPassiveMotionFunc(mouse_move);
    glutMotionFunc(mouse_move);
    //glutTimerFunc(25, updateScene, 0);

    // Show window & start update loop
    glutMainLoop();

    return 0;

}
