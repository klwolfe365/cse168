//
//  CellShading.h
//  cse168
//
// Written by Karen Wolfe
//
//
#ifndef CSE168_CELLSHADING_H_INCLUDED
#define CSE168_CELLSHADING_H_INCLUDED

#include "Lambert.h"

class CellShading : public Lambert
{
public:
    CellShading(const Vector3 & kd = Vector3(1),
            const Vector3 & ka = Vector3(0),
            float dr = 0.2, float mr = 0.6, float lr = 0.95,
            float di = 0.2, float mi = 0.6, float li = 0.8);
    virtual ~CellShading();

    void setDarkRange(float dr)     { darkRange = dr; }
    void setMidRange(float mr)      { midRange = mr; }
    void setLightRange(float lr)    { lightRange = lr; }

    void setDarkIntensity(float di) { darkIntensity = di; }
    void setMidIntensity(float mi)  { midIntensity = mi; }
    void setLightIntensity(float li){ lightIntensity = li; }


    virtual Vector3 shade(const Ray& ray, const HitInfo& hit,
                          const Scene& scene) const;
    Vector3 getCellColor(float nl) const;

protected:
    float darkRange;
    float midRange;
    float lightRange;
    float darkIntensity;
    float midIntensity;
    float lightIntensity;
};

#endif // CSE168_CELLSHADING_H_INCLUDED
