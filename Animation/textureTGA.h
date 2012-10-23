/*
    TGA texture loader
    © Keith O'Conor 2005
    keith.oconor @ [cs.tcd.ie | gmail.com]
*/

#ifndef TEXTURETGA_H
#define TEXTURETGA_H

#include <iostream>
#include <fstream>
#include <GL/freeglut.h>

typedef unsigned char byte;

typedef struct{
    byte idLength;
    byte colorMapType;
    byte imageType;
    byte colorMapInfo[5];
    byte x[2];
    byte y[2];
    byte width[2];
    byte height[2];
    byte bpp;    
}headerTga;

class textureTga{
private:
    int m_width;
    int m_height;
    int m_bpp;
    int m_format;
    
public:
    textureTga(const char *filename, const int textureId);
    
    int getWidth(){return m_width;}
    int getHeight(){return m_width;}
    int getBpp(){return m_bpp;}
};

#endif //TEXTURETGA_H
