//
//  Texture.h
//  cse168
//
//  Written by Karen Wolfe
//
//
#ifndef CSE168_TEXTURE_H_INCLUDED
#define CSE168_TEXTURE_H_INCLUDED

#include "Vector3.h"
#include "Image.h"

class Texture {
public:
	Texture() {}
	virtual ~Texture() {}

	virtual Vector3 getColor(Vector3& v) const { return Vector3(0.0f); }

	unsigned char * loadTexture(const char * filename);

	Image m_image;
	unsigned char * m_rawData;
};

#endif //CSE168_TEXTURE_H_INCLUDED
