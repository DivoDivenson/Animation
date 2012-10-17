//#include <windows.h>  // for timeGetTime()
//#include <mmsystem.h> // ditto
#include <iostream>     // I/O
//#include <glut.h>     // for gluPerspective & gluLookAt
#include <GL/freeglut.h>
#include <sys/time.h>

#include "Camera.h"
#include "model3DS.h"

#define START1 20
#define START2 100

void setupScene();
void updateScene();
void renderScene();
void exitScene();
void keypress(unsigned char key, int x, int y);
void setViewport(int width, int height);

typedef unsigned int DWORD;

Camera * camera;
model3DS * spider_body;

int         rotationAngle=0;
bool        wireframe=false;
int         windowId;
DWORD       lastTickCount;
GLfloat white_light[] = {0.5, 0.5, 0.5, 0.5};
GLfloat left_light_position[] = {1,0,-1, 1.0};
GLfloat right_light_position[] = {-1,0,-1, 1.0};

GLfloat emerald_ambient[] =
{0.0215, 0.1745, 0.0215}, emerald_diffuse[] =
{0.07568, 0.61424, 0.07568}, emerald_specular[] =
{0.633, 0.727811, 0.633}, emerald_shininess = 76.8;

float leg_angles[8][2];

int direction[] = {0, 1, 0, 1, 0, 1};
int side_direction[] = {1, 0, 1, 0, 1, 0};

int start_angles[] = {20, 100};

int side_angles[] = {0, 30, 0, 30, 0, 30};

float x, y;
int dir;




void render_leg(float direction, float pos, float &top_angle, float &bot_angle, int &vert_direction,
		int &side_angle, int &side_direction){


	//To setup the creation of quadric objects
    GLUquadric* nQ;
    nQ=gluNewQuadric();
    GLUquadric * nQ2;
    nQ2 = gluNewQuadric();

    //Movement for upper leg
    glPushMatrix();
    glTranslatef(0.0 , 0.0, -pos);

 	glRotatef(side_angle, 0, direction, 0);

    if(side_direction == 0)
    {
    	side_angle -= 1;
    }
    else if(side_direction == 1)
    {
    	side_angle += 1;
    }


    if(side_angle <= 0)
    {
    	side_direction = 1;
    }
    else if(side_angle >= 30)
    {
    	side_direction = 0;
    }



    //Movement for upper leg
    glRotatef(top_angle, 0, 0, 1);
    if(side_direction != 1)
    {
    	if(vert_direction == 0)
    	{
    		top_angle += 2;
    		bot_angle += 2;

    	}
    	else if(vert_direction == 1)
    	{
    		top_angle -= 2;
    		bot_angle -= 2;
    	}
    }


    if(top_angle >=  50.0f)
    {
    	vert_direction = 1;
    }
    else if(top_angle <= 20.0f)
    {
    	vert_direction = 0;
    }


    //angle += 2.0f;

    //Draw joint and upper leg
    glutSolidSphere(0.2f, 20, 20);
    glTranslatef(0.15f, 0, 0);
    glPushMatrix();
    	glRotatef(90.0f, 0, 1, 0);
    	gluCylinder(nQ, 0.15f, 0.10f, 1, 20, 5);
    glPopMatrix();



    //Draw join and mid leg
    glTranslatef(1.0f, 0, 0);
    glRotatef(bot_angle, 0, 0, -1);

    glutSolidSphere(0.2f, 20, 20);
    glPushMatrix();
    	glRotatef(90.0f, 0, 1, 0);
        gluCylinder(nQ, 0.12f, 0.08f, 1, 20, 5);
    glPopMatrix();

    glTranslatef(1.0f, 0, 0);
    glRotatef(10, 0, 0, 1);
    glutSolidSphere(0.1f, 20, 20);
    glPushMatrix();
       	glRotatef(90.0f, 0, 1, 0);
        gluCylinder(nQ, 0.05f, 0.01f, 0.3, 20, 5);
    glPopMatrix();

    glPopMatrix();


}


void render_forward_leg(float direction, float pos, float &top_angle, float &bot_angle, int &vert_direction)
{


	//To setup the creation of quadric objects
    GLUquadric* nQ;
    nQ=gluNewQuadric();
    GLUquadric * nQ2;
    nQ2 = gluNewQuadric();

    //Movement for upper leg
    glPushMatrix();
    glTranslatef(0.0 , 0.0, -pos);


    //Movement for upper leg
    glRotatef(top_angle, 0, 0, 1);

    if(vert_direction == 0)
    {
    	top_angle += 1;
    	bot_angle += 2;

    }
    else if(vert_direction == 1)
    {
    	top_angle -= 1;
    	bot_angle -= 2;
    }


    if(top_angle <=  10.0f)
    {
    	vert_direction = 0;
    }
    else if(top_angle >= 60.0f)
    {
    	vert_direction = 1;
    }


    //angle += 2.0f;

    //Draw joint and upper leg
    glutSolidSphere(0.2f, 20, 20);
    glTranslatef(0.15f, 0, 0);
    glPushMatrix();
    	glRotatef(90.0f, 0, 1, 0);
    	gluCylinder(nQ, 0.15f, 0.10f, 1, 20, 5);
    glPopMatrix();



    //Draw join and mid leg
    glTranslatef(1.0f, 0, 0);
    glRotatef(bot_angle, 0, 0, -1);

    glutSolidSphere(0.2f, 20, 20);
    glPushMatrix();
    	glRotatef(90.0f, 0, 1, 0);
        gluCylinder(nQ, 0.12f, 0.08f, 1, 20, 5);
    glPopMatrix();

    glTranslatef(1.0f, 0, 0);
    glRotatef(10, 0, 0, 1);
    glutSolidSphere(0.1f, 20, 20);
    glPushMatrix();
       	glRotatef(90.0f, 0, 1, 0);
        gluCylinder(nQ, 0.05f, 0.01f, 0.3, 20, 5);
    glPopMatrix();

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
    //camera->draw();
    glPushMatrix();
    //glEnable(GL_TEXTURE_2D);
    //Go back 10 along Z axis so we can see the models
    gluLookAt(0,0,10,  0,0,-1,  0,1,0);
    //Rotate so things look isometric
    glRotatef(45.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(45.0f, 0.0f, -1.0f, 0.0f);

    //at origin draw base
    //glutSolidCube(1.0f);

    glPushMatrix();
    glRotatef(90.0f, 0, 1, 0);
    spider_body->draw();
    glPopMatrix();

	render_forward_leg(1.0, -1, x, y, dir);


	//render_leg(1.0, 0, leg_angles[0][0], leg_angles[0][1], direction[0], side_angles[0], side_direction[0]);
	//render_leg(1.0, 1, leg_angles[1][0], leg_angles[1][1], direction[1], side_angles[1], side_direction[1]);
	//render_leg(1.0, 2, leg_angles[2][0], leg_angles[2][1], direction[2], side_angles[2], side_direction[2]);

	glRotatef(180.0f, 0, 1, 0);
	glPushMatrix();
	//render_leg(-1.0, 0, leg_angles[3][0], leg_angles[3][1], direction[3], side_angles[3], side_direction[3]);
	//render_leg(-1.0, -1, leg_angles[4][0], leg_angles[4][1], direction[4], side_angles[4], side_direction[4]);
	//render_leg(-1.0, -2, leg_angles[5][0], leg_angles[5][1], direction[5], side_angles[5], side_direction[5]);
	glPopMatrix();
	//render_leg(1);
	//render_leg(2);
	//glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    //glTranslatef(0, 0, 10);
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
    spider_body = new model3DS("./Assets/body.3ds", 1);



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

    leg_angles[0][0] = START1;
    leg_angles[0][1] = START2;

    leg_angles[1][0] = START1;
    leg_angles[1][1] = START2;

    leg_angles[2][0] = START1;
    leg_angles[2][1] = START2;

    leg_angles[3][0] = START1;
    leg_angles[3][1] = START2;

    leg_angles[4][0] = START1;
    leg_angles[4][1] = START2;

    leg_angles[5][0] = START1;
    leg_angles[5][1] = START2;

    //Temp hack
    x = 10;
    y = 40;
    dir = 0;

    camera = new Camera();
    // Setup OpenGL state & scene resources (models, textures etc)

    // Set GLUT callback functions
    glutReshapeFunc(setViewport);
    glutDisplayFunc(renderScene);
    glutIdleFunc(updateScene);
    glutKeyboardFunc(keypress);
    glutPassiveMotionFunc(mouse_move);
    glutMotionFunc(mouse_move);
    //glutTimerFunc(25, updateScene, 0);

    setupScene();


    // Show window & start update loop
    glutMainLoop();

    return 0;

}
