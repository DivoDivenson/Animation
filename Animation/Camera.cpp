/*
 * Camera.cpp
 *
 *  Created on: 14 Oct 2012
 *      Author: divo
 */
#include "Camera.h"

Camera::Camera() {
	xrot = 0;
	yrot = 0;

	button_presses[0] = 0;
	button_presses[1] = 0;
	button_presses[2] = 0;
	button_presses[3] = 0;

	reset_camera();


}

Camera::~Camera() {
	// TODO Auto-generated destructor stub
}


void Camera::move_player()
{
     
    if (xrot >360) xrot -= 360;
    

    if (xrot < -360) xrot += 360;

    if (button_presses[W]){
        float xtemp =0, ytemp =0, ztemp =0;

            float xrotrad, yrotrad;
            yrotrad = (yrot / 180 * GL_PI);
            xrotrad = (xrot / 180 * GL_PI);
            float adj_speed = float(button_presses[W] / 10.0f) * SPEED;
            xtemp = float(sin(yrotrad) * adj_speed);
            ztemp = float(cos(yrotrad) * adj_speed);
            ytemp = float(sin(xrotrad) * adj_speed);
            if(xpos > -TERRIAN_WIDTH + 0 && xpos < TERRIAN_WIDTH - 0){
                xpos += xtemp;
            }
            if(ypos > -TERRIAN_WIDTH + 0 && ypos < TERRIAN_WIDTH - 0){
                ypos -= ytemp;
            }
            if(zpos > -TERRIAN_WIDTH + 0 && zpos < TERRIAN_WIDTH - 0){
                zpos -= ztemp;
            }

            button_presses[W]--;
    }

    if (button_presses[S]){
        float xtemp =0, ytemp =0, ztemp =0;

            float xrotrad, yrotrad;
            yrotrad = (yrot / 180 * GL_PI);
            xrotrad = (xrot / 180 * GL_PI); 
                float adj_speed = float(button_presses[S] / 10.0f) * SPEED;

            xtemp = float(sin(yrotrad) * adj_speed);
            ztemp = float(cos(yrotrad) * adj_speed) ;
            ytemp = float(sin(xrotrad) * adj_speed);
            if(xpos > -TERRIAN_WIDTH + 0 && xpos < TERRIAN_WIDTH - 0){
                xpos -= xtemp;
            }
            if(ypos > -TERRIAN_WIDTH + 0 && ypos < TERRIAN_WIDTH - 0){
                ypos += ytemp;
            }
            if(zpos > -TERRIAN_WIDTH + 0 && zpos < TERRIAN_WIDTH - 0){
                zpos += ztemp;
            }
            button_presses[S]--;
    }

    if (button_presses[D]){
        float xtemp =0, ztemp =0;

        float yrotrad;
                yrotrad = (yrot / 180 * GL_PI);
                float adj_speed = float(button_presses[D] / 10.0f) * SPEED;

                xtemp = float(cos(yrotrad)) * adj_speed;
                ztemp = float(sin(yrotrad)) * adj_speed;
                if(xpos > -TERRIAN_WIDTH + 0 && xpos < TERRIAN_WIDTH - 0){
                xpos += xtemp;
            }
           
            if(zpos > -TERRIAN_WIDTH + 0 && zpos < TERRIAN_WIDTH - 0){
                zpos += ztemp;
            }
                button_presses[D]--;
    }

    if (button_presses[A]){
        float xtemp =0, ztemp =0;

                float yrotrad;
                yrotrad = (yrot / 180 * GL_PI);
                float adj_speed = float(button_presses[A] / 10.0f)* SPEED;

                xtemp = float(cos(yrotrad)) * adj_speed;
                ztemp = float(sin(yrotrad)) * adj_speed;
                if(xpos > -TERRIAN_WIDTH + 0 && xpos < TERRIAN_WIDTH - 0){
                xpos -= xtemp;
            }
           
            if(zpos > -TERRIAN_WIDTH + 0 && zpos < TERRIAN_WIDTH - 0){
                zpos -= ztemp;
            }
                button_presses[A]--;
    }
   

}

void Camera::keypressdown(unsigned char key, int x, int y){
        if(key == 'w'){
                button_presses[W] = 10;
        }
        if(key == 'a'){
                button_presses[A] = 10;
        }
        if(key == 's'){
                button_presses[S] = 10;
        }
        if(key == 'd'){
                button_presses[D] = 10;
        }
        if(key == 'r'){
			reset_camera();    
        }
        if (key==27)
    {
    exit(0);
    }

}

void Camera::reset_camera()
{
	 xpos = TERRIAN_WIDTH / 2;
     zpos = TERRIAN_WIDTH / 2;
     ypos = 0;//TERRIAN_WIDTH / 2;
}

void Camera::keypressup(unsigned char key, int x, int y){
        if(key == 'w'){
                button_presses[W] = 9;
        }
        if(key == 'a'){
                button_presses[A] = 9;
        }
        if(key == 's'){
                button_presses[S] = 9;
        }
        if(key == 'd'){
                button_presses[D] = 9;
        }
		

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
