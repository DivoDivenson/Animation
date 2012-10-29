#pragma once

#include "textureTGA.h"

enum texture_ints { t_xneg, t_xpos, t_yneg, t_ypos, t_zneg, t_zpos}; //For the skybox


class SkyBox
{
private:
	float width;
	float border;

public:
	SkyBox(float i_width, float i_border);
	void draw(void);
	~SkyBox(void);

	

};

