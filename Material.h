#ifndef CSE168_MATERIAL_H_INCLUDED
#define CSE168_MATERIAL_H_INCLUDED

#include "Miro.h"
#include "Vector3.h"

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

protected:
    float m_n; //index of refraction - default air
    static const int MAX_BOUNCE = 5;
};

#endif // CSE168_MATERIAL_H_INCLUDED
