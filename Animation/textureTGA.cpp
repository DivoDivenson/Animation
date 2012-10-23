/*
    TGA texture loader
    © Keith O'Conor 2005
    keith.oconor @ [cs.tcd.ie | gmail.com]
*/

#include "textureTga.h"

textureTga::textureTga(const char *filename, const int textureId){
    byte *imageData;
    int numPixels;
    int bytesInPixel;
    int imageDataSize;
    headerTga fileHeader;
    byte temp;  
    
    // Open file
    std::ifstream textureFile(filename, std::ios::in | std::ios::binary | std::ios::ate);
    
    if(!textureFile.is_open()){
        std::cout<<"[TGA] ERROR: Could not open '"<<filename<<"'"<<std::endl;
        return;   
    }
    if(int(textureFile.tellg()) == 0){
        std::cout<<"[TGA] ERROR: Texture '"<<filename<<"' is empty"<<std::endl;
        textureFile.close();
        return;
    }  
    
    // Read TGA header
    textureFile.seekg(0, std::ios::beg);
    textureFile.read((char*)&fileHeader, sizeof(fileHeader));
    
    m_width = fileHeader.width[0] + (fileHeader.width[1] << 8);
    m_height = fileHeader.height[0] + (fileHeader.height[1] << 8);
    m_bpp = fileHeader.bpp;

    // We only support uncompressed 24 or 32 bits per pixel TGAs
    if(fileHeader.colorMapType == 1 || int(fileHeader.imageType) != 2){
        std::cout<<"[TGA] ERROR: Invalid texture format, '"<<filename<<"' must be uncompressed 24/32bpp TGA"<<std::endl;
        textureFile.close();
        return;
    }
    if(m_bpp != 32 && m_bpp != 24){
        std::cout<<"[TGA] ERROR: Invalid texture color depth, '"<<filename<<"' must be uncompressed 24/32bpp TGA"<<std::endl;
        textureFile.close();
        return;
    }
    
    // OpenGL textures must be power-of-two
    if((m_width&(m_width-1)) || (m_height&(m_height-1))){
        std::cout<<"[TGA] ERROR: Texture '"<<filename<<"' must have power-of-two width & height"<<std::endl;
        textureFile.close();
        return;        
    }
    
    // Determine format
    switch(m_bpp){
        case 24:m_format = GL_RGB;  bytesInPixel = 3; break;   
        case 32:m_format = GL_RGBA; bytesInPixel = 4; break;
        default:
            std::cout<<"[TGA] ERROR: Invalid texture color depth, '"<<filename<<"' must be uncompressed 24/32bpp TGA"<<std::endl;
            textureFile.close();
            return;
            break;
    }
    
    // Allocate memory for image    
    numPixels = m_width * m_height;
    imageDataSize = numPixels * bytesInPixel;
    imageData = new byte[imageDataSize];
    if(!imageData){
        std::cout<<"[TGA] ERROR: Out of memory"<<std::endl;
        textureFile.close();
        return;
    }
    
    // Read image data
    textureFile.seekg(18, std::ios::beg);
    textureFile.read((char*)imageData, imageDataSize);
    textureFile.close();
      
    // TGAs are BGRA, convert to RGBA
    for(int pixel=0; pixel < numPixels*bytesInPixel; pixel+=bytesInPixel){
        temp = imageData[pixel+2];
        imageData[pixel+2] = imageData[pixel];
        imageData[pixel] = temp;
    }
    
    // Bind texture ID to load
    glBindTexture(GL_TEXTURE_2D, textureId);
    
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    
    // Upload texture to card with bound texture ID
    glTexImage2D(GL_TEXTURE_2D, 0, m_format, m_width, m_height, 0, m_format, GL_UNSIGNED_BYTE, imageData);
    
    // Texture's uploaded, don't need data any more   
    delete imageData;
    
    std::cout<<"[TGA] Texture '"<<filename<<"' loaded"<<std::endl;
}
