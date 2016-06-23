//
//  Texture.cpp
//  cse168
//
//  Written by Karen Wolfe
//
//
#include "Texture.h"
#include "Image.h"

unsigned char * Texture::loadTexture(const char * filename){
    m_rawData = m_image.readPPM(filename);
    return m_rawData;

}
