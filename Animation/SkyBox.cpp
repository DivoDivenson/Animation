#include "SkyBox.h"





SkyBox::SkyBox(float i_width, float i_border)
{
	width = i_width;
	border = i_border;
	textureTga("./Skybox/xneg.tga", t_xneg);
    textureTga("./SkyBox/xpos.tga", t_xpos);
    textureTga("./SkyBox/yneg.tga", t_yneg);
    textureTga("./SkyBox/ypos.tga", t_ypos);
    textureTga("./SkyBox/zneg.tga", t_zneg);
    textureTga("./SkyBox/zpos.tga", t_zpos);
}

void SkyBox::draw(){
    glPushMatrix();
    // Enable/Disable features
    glPushAttrib(GL_ENABLE_BIT);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_BLEND);

 
    // Just in case we set all vertices to white.
    //glColor4f(1,1,1,1);
    glBindTexture(GL_TEXTURE_2D, t_xneg);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(  width, -border, -border );
        glTexCoord2f(1, 0); glVertex3f( width, -border, width  );
        glTexCoord2f(1, 1); glVertex3f( width,  width, width );
        glTexCoord2f(0, 1); glVertex3f(  width,  width, -border );
    glEnd();

    glBindTexture(GL_TEXTURE_2D, t_xpos);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(  -border, -border, width );
        glTexCoord2f(1, 0); glVertex3f( -border, -border, -border  );
        glTexCoord2f(1, 1); glVertex3f( -border,  width, -border );
        glTexCoord2f(0, 1); glVertex3f(  -border,  width, width );
    glEnd();

    glBindTexture(GL_TEXTURE_2D, t_zneg);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(  -border, -border, -border );
        glTexCoord2f(1, 0); glVertex3f( width, -border, -border  );
        glTexCoord2f(1, 1); glVertex3f( width,  width, -border );
        glTexCoord2f(0, 1); glVertex3f(  -border,  width, -border );
    glEnd();

    glBindTexture(GL_TEXTURE_2D, t_zpos);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(  width, -border, width );
        glTexCoord2f(1, 0); glVertex3f( -border, -border, width );
        glTexCoord2f(1, 1); glVertex3f( -border,  width, width );
        glTexCoord2f(0, 1); glVertex3f(  width,  width, width );
    glEnd();

    glBindTexture(GL_TEXTURE_2D, t_yneg);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(  width, -border, -border );
        glTexCoord2f(1, 0); glVertex3f( width, -border, width );
        glTexCoord2f(1, 1); glVertex3f( -border,  -border, width );
        glTexCoord2f(0, 1); glVertex3f(  -border,  -border, -border );
    glEnd();

    glBindTexture(GL_TEXTURE_2D, t_ypos);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(  width,  width, -border );
        glTexCoord2f(1, 0); glVertex3f(  -border, width, -border );
        glTexCoord2f(1, 1); glVertex3f( -border, width, width );
        glTexCoord2f(0, 1); glVertex3f( width,  width, width );
    glEnd();

    glEnable(GL_LIGHTING);

    glPopAttrib();
    glPopMatrix();
 }


SkyBox::~SkyBox(void)
{
}
