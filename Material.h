//
//  Material.h
//  cse168
//
//  Provided in CSE168 starter code
//  Modified by Karen Wolfe
//
//
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

    virtual Vector3 BRDF(const Vector3& in, const Vector3& normal, const Vector3& out, const bool& isFront = true) const { return 0; }
    virtual Vector3 reflect(const Vector3 & normal, const Vector3 & incident) const;
    virtual Vector3 refract(const Vector3 & normal, const Vector3 & incident,
        double n1, double n2) const;

    virtual const float n() const {return m_n;}
    virtual const Texture* texture() const        { return m_texture; }
    virtual void setTexture(Texture * t)  { m_texture = t;}

    virtual const bool hasTexture() const   { return m_texture != NULL;}

    //Used for monte-carlo algorithms
    float emittance() const { return m_emittance; }
    void setEmittance(const float& e) { m_emittance = e; }
    virtual Vector3 reflectance() const { return Vector3(0,0,0); }
    virtual Vector3 randomReflect(const Vector3& ray, const Vector3& hit, const bool& isFront = true);
    virtual float getEmittance(const Ray& ray, const HitInfo& hit,
                          const Scene& scene) const { return 0.2; }

protected:
    float m_n; //index of refraction - default air
    static const int MAX_BOUNCE = 5;
    Texture * m_texture;
    float m_emittance;
};

#endif // CSE168_MATERIAL_H_INCLUDED
