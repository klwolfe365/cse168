#ifndef CSE168_MATERIAL_H_INCLUDED
#define CSE168_MATERIAL_H_INCLUDED

#include "Miro.h"
#include "Vector3.h"
#include "Texture.h"

class Material
{
public:
    Material();
    virtual ~Material();

    virtual void preCalc() {}

    virtual Vector3 shade(const Ray& ray, const HitInfo& hit,
                          const Scene& scene) const;

    virtual Vector3 reflect(const Vector3 & normal, const Vector3 & incident) const;
    virtual Vector3 refract(const Vector3 & normal, const Vector3 & incident,
        double n1, double n2) const;

    virtual const float n() const {return m_n;}
    virtual const Texture* texture() const        { return m_texture; }
    virtual void setTexture(Texture * t)  { m_texture = t;}

    virtual const bool hasTexture() const   { return m_texture != NULL;}

protected:
    float m_n; //index of refraction - default air
    static const int MAX_BOUNCE = 5;
    Texture * m_texture;
};

#endif // CSE168_MATERIAL_H_INCLUDED
