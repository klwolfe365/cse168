#ifndef CSE168_BVH_H_INCLUDED
#define CSE168_BVH_H_INCLUDED

#include "Miro.h"
#include "Object.h"
#include "BoundingBox.h"

class BVH
{
public:
    static const int NUM_LEAF_NODES = 8;
    void build(Objects * objs);

    bool intersect(HitInfo& result, const Ray& ray,
                   float tMin = 0.0f, float tMax = MIRO_TMAX) const;
    BoundingBox getBoundingBox() { return bbox; }

    void draw();

    int getLeafNum();
    int getNodeNum();
    int getBoundingBoxHits();
    int getTriangleHits();

protected:
    Objects * m_objects;
    BVH* left_child;
    BVH* right_child;
    BoundingBox bbox;
    bool isLeaf;
};

#endif // CSE168_BVH_H_INCLUDED
