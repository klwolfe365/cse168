//
//  CoolWarmShader.h
//  cse168
//
//  Written by Karen Wolfe
//
//
#ifndef CSE168_COOLWARMSHADER_H_INCLUDED
#define CSE168_COOLWARMSHADER_H_INCLUDED

#include "Lambert.h"

class CoolWarmShader : public Lambert
{
public:
    CoolWarmShader(const Vector3 & cool = Vector3(0.4, 0.4, 0.7),
            const Vector3 & warm = Vector3(0.8, 0.6, 0.6), const bool edges = false);
    virtual ~CoolWarmShader();

    const Vector3 & cool() const {return m_cool;}
    const Vector3 & warm() const {return m_warm;}

    void setCoolColor(const Vector3 & c) { m_cool = c; }
    void setWarmColor(const Vector3 & w) { m_warm = w; }

    virtual Vector3 shade(const Ray& ray, const HitInfo& hit,
                          const Scene& scene) const;
    Vector3 getCellColor(float nl) const;

protected:
    Vector3 m_warm;
    Vector3 m_cool;
    bool m_edges;
};

#endif // CSE168_COOLWARMSHADER_H_INCLUDED
