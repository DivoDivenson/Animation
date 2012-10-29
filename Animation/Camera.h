#include <iostream>

#include <math.h>
#include <GL/freeglut.h>
#include "consts.h"

class Camera {
public:

	float xpos, ypos, zpos;
	float xrot, yrot;
	float lastx, lasty;

	Camera();
	virtual ~Camera();
	void move_player();
	void mouse_move(int x, int y);
	void draw();

};

