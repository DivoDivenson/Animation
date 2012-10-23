#include <windows.h>	// for timeGetTime()
#include <mmsystem.h>	// ditto
#include <iostream>		// I/O
#include <GL/freeglut.h>
#include "textureTGA.h" // TGA support

void setupScene();
void updateScene();
void renderScene();
void exitScene();
void keypress(unsigned char key, int x, int y);
void setViewport(int width, int height);

int         tetrahedronAngle=0;
bool		wireframe=false;
int         windowId;
GLuint      textureId;
DWORD		lastTickCount;
    
void renderScene(){
        
    // Clear framebuffer & depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Reset Modelview matrix      	
    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	// Set view position & direction
	// (Camera at (0,0,5) looking down the negative Z-axis)
	gluLookAt(0,0,5,  0,0,-1,  0,1,0);

    // Draw textured tetrahedron
    glColor3f(1,1,1);
    glBindTexture(GL_TEXTURE_2D,textureId);

    glPushMatrix();
		glRotatef(tetrahedronAngle/4.f,0,1,0);
        glRotatef(tetrahedronAngle/3.f,1,0,0);
        glRotatef(tetrahedronAngle/2.f,0,0,1);

		//4 different faces, counter-clockwise vertex winding
		glBegin(GL_TRIANGLES);
            glTexCoord2f(.5,1);glVertex3f(-1, 1,-1);
            glTexCoord2f(0,0);glVertex3f(1, 1, 1);
            glTexCoord2f(1,0);glVertex3f(1,-1,-1);
            
            glTexCoord2f(.5,1);glVertex3f(-1,-1, 1);
            glTexCoord2f(0,0);glVertex3f(-1, 1,-1);
            glTexCoord2f(1,0);glVertex3f(1,-1,-1);
            
            glTexCoord2f(.5,1);glVertex3f(1,-1,-1);
            glTexCoord2f(0,0);glVertex3f(1, 1, 1);
            glTexCoord2f(1,0);glVertex3f(-1,-1, 1);
            
            glTexCoord2f(.5,1);glVertex3f(-1,-1, 1);
            glTexCoord2f(0,0);glVertex3f(1, 1 ,1);
            glTexCoord2f(1,0);glVertex3f(-1, 1,-1);
        glEnd();

	glPopMatrix();
   
    
    // Swap double buffer for flicker-free animation
    glutSwapBuffers();
        
}

void updateScene(){
	
	// Wait until at least 16ms passed since start of last frame
	// Effectively caps framerate at ~60fps
	while(timeGetTime()-lastTickCount<16);
	lastTickCount=timeGetTime();
    
    // Increment angle for next frame
    tetrahedronAngle+=2;

	// Do any other updates here
	
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
    
    // Initialise desired OpenGL state
    glClearColor(0,0,0,0);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // Generate GL texture ID & load texture
    glGenTextures(1, &textureId);
    textureTga logoTexture("isglogo.tga", textureId);
        
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
    glutInitWindowSize(640,480);
    windowId = glutCreateWindow("Example GLUT app");
    
    // Set GLUT callback functions
    glutReshapeFunc(setViewport);
    glutDisplayFunc(renderScene);
    glutIdleFunc(updateScene);
    glutKeyboardFunc(keypress);

    // Setup OpenGL state & scene resources (models, textures etc)
    setupScene();

    // Show window & start update loop
    glutMainLoop();    

    return 0;
    
}
