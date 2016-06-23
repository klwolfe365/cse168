//
//  PencilShader.h
//  cse168
//
//  Written by Karen Wolfe
//
//
#ifndef CSE168_PENCILSHADER_H_INCLUDED
#define CSE168_PENCILSHADER_H_INCLUDED

#include <vector>
#include "Lambert.h"

#define M_TEXTURES 7

class PencilShader : public Lambert
{
public:
    PencilShader(const Vector3 & kd = Vector3(1),
            const Vector3 & ka = Vector3(0));
    virtual ~PencilShader();


    virtual Vector3 shade(const Ray& ray, const HitInfo& hit,
                          const Scene& scene) const;
    Vector3 getTextureColor(float nl, const HitInfo& hit) const;
    void loadTexture(const char * filename);

    void loadTextures(std::vector<const char * > filenames);
protected:
    std::vector<Texture* > textures;
};

#endif // CSE168_PENCILSHADER_H_INCLUDED
