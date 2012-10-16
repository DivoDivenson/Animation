/*
 * Camera.cpp
 *
 *  Created on: 14 Oct 2012
 *      Author: divo
 */
#include <iostream>

#include <math.h>
#include <GL/freeglut.h>
#include "Camera.h"
#include "consts.h"

Camera::Camera() {
	xrot = 0;
	yrot = 0;
}

Camera::~Camera() {
	// TODO Auto-generated destructor stub
}


void Camera::move_player()
{
	if(xrot > 360)
		xrot -= 360;

	if(xrot < -360)
		xrot += 360;

	float xrotrad, yrotrad;
	xrotrad = (xrot / 180 * M_PI);
	yrotrad = (yrot / 180 * M_PI);

	xpos = float(sin(yrotrad) * SPEED);
	zpos = float(cos(yrotrad) * SPEED);
	ypos = float(sin(xrotrad) * SPEED);

}

void Camera::draw()
{
	glRotatef(xrot, 1.0f, 0.0f, 0.0f);
	glRotatef(yrot, 0.0f, 1.0f, 0.0f);
	glTranslatef(-xpos, -ypos, -zpos);
}

void Camera::mouse_move(int x, int y)
{
	int diffx=x-lastx; //check the difference between the current x and the last x position
	int diffy=y-lasty; //check the difference between the current y and the last y position
	lastx=x; //set lastx to the current x position
	lasty=y; //set lasty to the current y position
	xrot += (float) diffy; //set the xrot to xrot with the addition of the difference in the y position
	yrot += (float) diffx;// set the xrot to yrot with the addition of the difference in the x position
	if(yrot > 360.0f){
		yrot -= 360.0f;
	}
}

