//
//  StoneTexture.h
//  cse168
//
//  Written by Karen Wolfe
//
//
#ifndef CSE168_STONETEXTURE_H_INCLUDED
#define CSE168_STONETEXTURE_H_INCLUDED

#include "Texture.h"

class StoneTexture : public Texture
{
public:
    StoneTexture(const float size = 0.0f, const int order = 0);
    virtual ~StoneTexture();

    virtual Vector3 getColor(Vector3& v) const;

    void setOrder(const int o)      { order = o; }
    void setSize(const float s)     { size = s; }

    const int getOrder() const      { return order; }
    const float getSize() const     { return size; }

protected:
    float size;
    int order;
};
#endif // CSE168_STONETEXTURE_H_INCLUDED
