/*
    TGA texture loader
    © Keith O'Conor 2005
    keith.oconor @ {cs.tcd.ie, gmail.com}
*/

#ifndef TEXTURETGA_H
#define TEXTURETGA_H

#undef UNICODE
#define WIN32_LEAN_AND_MEAN
//#include <windows.h>
#include <iostream>
#include <fstream>
#include "GL/gl.h"
#include "GL/glu.h"
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

typedef unsigned char byte;

class textureTGA{
private:
    int m_width;
    int m_height;
    int m_bpp;
    int m_format;
    
public:
    textureTGA(const char *filename, const int textureId);
	textureTGA(std::string filename, const int textureId){textureTGA(filename.c_str(),textureId);}
    
    int getWidth(){return m_width;}
    int getHeight(){return m_width;}
    int getBpp(){return m_bpp;}
};

#endif //TEXTURETGA_H
