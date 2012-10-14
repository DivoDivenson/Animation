/*
 * Camera.h
 *
 *  Created on: 14 Oct 2012
 *      Author: divo
 */



class Camera {
public:

	float xpos, ypos, zpos;
	float xrot, yrot;
	float lastx, lasty;

	Camera();
	virtual ~Camera();
	void move_player();
	void mouse_move(int x, int y);

};


