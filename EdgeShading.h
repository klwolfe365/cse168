//
//  EdgeShading.h
//  cse168
//
//  Written by Karen Wolfe
//
//
#ifndef CSE168_EDGESHADING_H_INCLUDED
#define CSE168_EDGESHADING_H_INCLUDED

#include "CellShading.h"

class EdgeShading : public CellShading
{
public:
    EdgeShading(const Vector3 & kd = Vector3(1),
            const Vector3 & ka = Vector3(0),
            float dr = 0.2, float mr = 0.6, float lr = 0.9,
            float di = 0.2, float mi = 0.6, float li = 0.8, float edgeWeight = 0.1);
    EdgeShading(float edgeweight);
    virtual ~EdgeShading();

    virtual Vector3 shade(const Ray& ray, const HitInfo& hit,
                          const Scene& scene) const;
    float rayStencilRegion(const Ray& ray, const HitInfo& hit, const Scene& scene) const;
    Ray sampleRegion(const Ray& ray);

protected:
    float weight;
};

#endif // CSE168_EDGESHADING_H_INCLUDED
