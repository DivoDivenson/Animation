#include <iostream>

#include <math.h>
#include <GL/freeglut.h>
#include "consts.h"

enum BUTTONS { W, A, S, D };


class Camera {
private:
	void reset_camera();

public:

	float xpos, ypos, zpos;
	float xrot, yrot;
	float lastx, lasty;
	int button_presses[4];

	Camera();
	virtual ~Camera();
	void move_player();
	void mouse_move(int x, int y);
	void draw();
	void keypressdown(unsigned char key, int x, int y);
	void keypressup(unsigned char key, int x, int y);


};

