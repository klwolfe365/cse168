//
//  Lambert.h
//  cse168
//
//  Provided in CSE168 starter code
//  Modified by Karen Wolfe
//
//
#ifndef CSE168_LAMBERT_H_INCLUDED
#define CSE168_LAMBERT_H_INCLUDED

#include "Material.h"

class Lambert : public Material
{
public:
    Lambert(const Vector3 & kd = Vector3(1),
            const Vector3 & ka = Vector3(0));
    virtual ~Lambert();

    const Vector3 & kd() const {return m_kd;}
    const Vector3 & ka() const {return m_ka;}

    void setKd(const Vector3 & kd) {m_kd = kd;}
    void setKa(const Vector3 & ka) {m_ka = ka;}

    virtual void preCalc() {}

    Vector3 BRDF(const Vector3& in, const Vector3& normal, const Vector3& out, const bool& isFront = true) const;
    virtual Vector3 shade(const Ray& ray, const HitInfo& hit,
                          const Scene& scene) const;
    virtual Vector3 reflect(const Vector3 & normal, const Vector3 & incident) const;
    virtual Vector3 refract(const Vector3 & normal, const Vector3 & incident,
                  double n1, double n2) const;
    virtual Vector3 reflectance() const { return m_kd; }
    virtual Vector3 randomReflect(Vector3 in, Vector3 hitNorm);
    virtual float getEmittance(const Ray& ray, const HitInfo& hit,
                          const Scene& scene) const;
protected:
    Vector3 m_kd;
    Vector3 m_ka;
};

#endif // CSE168_LAMBERT_H_INCLUDED
