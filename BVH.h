#ifndef CSE168_BVH_H_INCLUDED
#define CSE168_BVH_H_INCLUDED

#include "Miro.h"
#include "Object.h"
#include "BoundingBox.h"

class BVH
{
public:
    void build(Objects * objs);

    bool intersect(HitInfo& result, const Ray& ray,
                   float tMin = 0.0f, float tMax = MIRO_TMAX) const;
    BoundingBox getBoundingBox() { return bbox; }

    void draw();

protected:
    Objects * m_objects;
    BVH* left_child;
    BVH* right_child;
    BoundingBox bbox;
    bool isLeaf;
};

#endif // CSE168_BVH_H_INCLUDED
