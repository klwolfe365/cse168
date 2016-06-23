//
//  Specular.h
//  cse168
//
//  Written by Karen Wolfe
//
//
#ifndef CSE168_SPECULAR_H_INCLUDED
#define CSE168_SPECULAR_H_INCLUDED

#include "Material.h"

class Specular : public Material
{
public:
    Specular(const Vector3 & kd = Vector3(1),
             const Vector3 & ks = Vector3(0),
             const float p = 1.0f,
             const float rl = 0.0f,
             const float n = 1.0f,
             const float rf = 0.0f
         );
    virtual ~Specular();

    const Vector3 & kd() const {return m_kd;}
    const Vector3 & ks() const {return m_ks;}
    const float p() const {return m_p;}
    const float rl() const {return m_rl;}
    const float rf() const {return m_rf;}

    void setKd(const Vector3 & kd) {m_kd = kd;}
    void setKs(const Vector3 & ks) {m_ks = ks;}
    void setP(const float p) {m_p = p;}
    void setRl(const float rl) {m_rl = rl;}
    void setRf(const float rf) {m_rf = rf;}


    virtual void preCalc() {}

    virtual Vector3 shade(const Ray& ray, const HitInfo& hit,
                          const Scene& scene) const;
    virtual Vector3 reflect(const Vector3 & normal, const Vector3 & incident) const;
    virtual Vector3 refract(const Vector3 & normal, const Vector3 & incident,
                    double n1, double n2) const;
protected:
    Vector3 m_kd;   //diffuse color
    Vector3 m_ks;   //specular reflection color
    float m_p;      //phong exponent
    float m_rl;     //reflection coefficient
    float m_rf;     //refraction coefficient
};
#endif //CSE168_SPECULAR_H_INCLUDED
