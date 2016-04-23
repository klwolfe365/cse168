#ifndef CSE168_SPECULAR_H_INCLUDED
#define CSE168_SPECULAR_H_INCLUDED

#include "Material.h"

class Specular : public Material
{
public:
    Specular(const Vector3 & kd = Vector3(1),
             const Vector3 & ks = Vector3(0),
             const float n = 1.0f);
    virtual ~Specular();

    const Vector3 & kd() const {return m_kd;}
    const Vector3 & ks() const {return m_ks;}
    const float n() const {return m_n;}

    void setKd(const Vector3 & kd) {m_kd = kd;}
    void setKs(const Vector3 & ks) {m_ks = ks;}
    void setN(const float n) {m_n = n;}

    virtual void preCalc() {}

    virtual Vector3 shade(const Ray& ray, const HitInfo& hit,
                          const Scene& scene) const;
protected:
    Vector3 m_kd;
    Vector3 m_ks;
    float m_n;
    static const int MAX_BOUNCE = 5;
};
#endif //CSE168_SPECULAR_H_INCLUDED
