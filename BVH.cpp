#include "BVH.h"
#include "Ray.h"
#include "Console.h"
//Helper methods for calculating cost, based of bin sort suggestion in lecture
struct bin {
    int numObjects;
    Vector3 binMin;
    Vector3 binMax;
};

float getSurfaceArea(Vector3 bMin, Vector3 bMax){
    float xwidth = bMax.x - bMin.x;
    float ywidth = bMax.y - bMin.y;
    float zwidth = bMax.z - bMin.z;
    return 2 *((xwidth*ywidth) + (xwidth*zwidth) + (ywidth*zwidth));
}

float getSplitPlaneCost(bin *b, int numPartitions, int split){
    int lNum = 0;
    int rNum = 0;

    for(int i = 0; i < numPartitions; i++){
        if(i <= split)
            lNum += b[i].numObjects;
        else
            rNum += b[i].numObjects;
    }

    float lCost = getSurfaceArea(b[0].binMin, b[split].binMax) * lNum;
    float rCost = getSurfaceArea(b[split+1].binMin, b[numPartitions - 1].binMax) * rNum;
    return lCost + rCost;
}
////////////////////////////////////////////////////////////////////////////////
void
BVH::build(Objects * objs)
{
    // construct the bounding volume hierarchy
    m_objects = objs;
    isLeaf = false;
    Vector3 boxMin;
    Vector3 boxMax;

    Objects * leftObjects;
    Objects * rightObjects;

    left_child = new BVH();
    right_child = new BVH();
    left_child->build(leftObjects);
    right_child->build(rightObjects);
}


bool
BVH::intersect(HitInfo& minHit, const Ray& ray, float tMin, float tMax) const
{
    // Here you would need to traverse the BVH to perform ray-intersection
    // acceleration. For now we just intersect every object.

    bool hit = false;
    HitInfo tempMinHit;
    // tempMinHit.hitNum = minHit.hitNum;
    minHit.t = MIRO_TMAX;

    for (size_t i = 0; i < m_objects->size(); ++i)
    {
        if ((*m_objects)[i]->intersect(tempMinHit, ray, tMin, tMax))
        {
            hit = true;
            if (tempMinHit.t < minHit.t){
                // tempMinHit.hitNum = minHit.hitNum;
                minHit = tempMinHit;
                minHit.material = tempMinHit.material;
            }
        }
    }

    return hit;
}
