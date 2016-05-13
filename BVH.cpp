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

/* Given a split plane number (0->number of partitions), calculates
 * the cost  of dividing a bounding box at that point
 */
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
    Vector3 objMin;
    Vector3 objMax;

    //Loop to set up bounding box widths
    for(int i = 0; i < m_objects->size(); i++){
        Vector3 tempMin = m_objects->at(i)->getMin();
        Vector3 tempMax = m_objects->at(i)->getMax();
        Vector3 objCenter = m_objects->at(i)->getCenter();

        // Setting the overall widths for the current bounding box
        if(tempMin.x < boxMin.x)
            boxMin.x = tempMin.x;
        if(tempMin.y < boxMin.y)
            boxMin.y = tempMin.y;
        if(tempMin.z < boxMin.z)
            boxMin.z = tempMin.z;

        if(tempMax.x > boxMin.x)
            boxMax.x = tempMax.x;
        if(tempMax.y > boxMax.y)
            boxMax.y = tempMax.y;
        if(tempMax.z > boxMax.z)
            boxMax.z = tempMax.z;

        // Object centers used for creating partition widths
        if(objCenter.x < objMin.x)
            objMin.x = objCenter.x;
        if(objCenter.y < objMin.y)
            objMin.y = objCenter.y;
        if(objCenter.z < objMin.z)
            objMin.z = objCenter.z;

        if(objCenter.x > objMax.x)
            objMax.x = objCenter.x;
        if(objCenter.y > objMax.y)
            objMax.y = objCenter.y;
        if(objCenter.z > objMax.z)
            objMax.z = objCenter.z;
    }

    // Create bounding box for this BVH node, set leaf status
    bbox = BoundingBox(boxMin, boxMax);
    if(m_objects->size() <= NUM_LEAF_NODES){
        isLeaf = true;
        return;
    }

    //Bucket sort based separation of objects into children nodes
    const int numPartitions = 4;
    float segmentWidthX = (objMax.x-objMin.x)/numPartitions;
    float segmentWidthY = (objMax.y-objMin.y)/numPartitions;
    float segmentWidthZ = (objMax.z-objMin.z)/numPartitions;
    bin binsX[numPartitions];
    bin binsY[numPartitions];
    bin binsZ[numPartitions];

    //Set up bins using partition segment widths in x/y/z planes
    for(int i = 0; i < numPartitions; i++){
        binsX[i].numObjects = 0;
        binsX[i].binMin = objMin;
        binsX[i].binMax = objMax;
        binsX[i].binMin.x = objMin.x + (i*segmentWidthX); //bin starting point
        binsX[i].binMin.x = objMin.x + ((i+1)*segmentWidthX); //bin ending point

        binsY[i].numObjects = 0;
        binsY[i].binMin = objMin;
        binsY[i].binMax = objMax;
        binsY[i].binMin.y = objMin.y + (i*segmentWidthY); //bin starting point
        binsY[i].binMin.y = objMin.y + ((i+1)*segmentWidthY); //bin ending point

        binsZ[i].numObjects = 0;
        binsZ[i].binMin = objMin;
        binsZ[i].binMax = objMax;
        binsZ[i].binMin.z = objMin.z + (i*segmentWidthZ); //bin starting point
        binsZ[i].binMin.z = objMin.z + ((i+1)*segmentWidthZ); //bin ending point
    }
    //Populate bins
    bool hasX = false;
    bool hasY = false;
    bool hasZ = false;
    for(int i = 0; i < m_objects->size(); i++){
        Vector3 center = m_objects->at(i)->getCenter();
        hasX = hasY = hasZ = false;
        for(int p= 0; p < numPartitions; p++){
            if(!hasX && center.x <= binsX[p].binMax.x){
                binsX[p].numObjects++;
                hasX = true;
            }
            if(!hasY && center.y <= binsY[p].binMax.y){
                binsY[p].numObjects++;
                hasY = true;
            }
            if(!hasZ && center.z <= binsZ[p].binMax.z){
                binsZ[p].numObjects++;
                hasZ = true;
            }
        }
    }

    //Calculate costs to pick ideal splitting plane
    float xCost = getSplitPlaneCost(binsX, numPartitions, 0);
    float yCost = getSplitPlaneCost(binsY, numPartitions, 0);
    float zCost = getSplitPlaneCost(binsZ, numPartitions, 0);
    int splitX = 0;
    int splitY = 0;
    int splitZ = 0;
    float currCost = 0.0f;
    for(int i = 1; i < numPartitions; i++){
        currCost = getSplitPlaneCost(binsX, numPartitions, i);
        if(currCost < xCost){
            splitX = i;
            xCost = currCost;
        }

        currCost = getSplitPlaneCost(binsY, numPartitions, i);
        if(currCost < yCost){
            splitY = i;
            yCost = currCost;
        }

        currCost = getSplitPlaneCost(binsZ, numPartitions, i);
        if(currCost < zCost){
            splitZ = i;
            zCost = currCost;
        }
    }
    //Determine the optimal splitting plane
    float splitPlane = splitX;
    char splitAxis = 'x';
    float minCost = fmin(xCost, fmin(yCost, zCost));
    if(minCost == yCost){
        splitPlane = splitY;
        splitAxis = 'y';
    } else if(minCost == zCost){
        splitPlane = splitZ;
        splitAxis = 'z';
    }

    //Divide objects along splitting plane based off center
    Objects * leftObjects;
    Objects * rightObjects;
    for(int i = 0; i < m_objects->size(); i++){
        Vector3 currCenter = m_objects->at(i)->getCenter();
        switch (splitAxis) {
            case 'x': {
                if(currCenter.x <= binsX[splitAxis].binMax.x)
                    leftObjects->push_back(m_objects->at(i));
                else
                    rightObjects->push_back(m_objects->at(i));
                break;
            }
            case 'y': {
                if(currCenter.y <= binsX[splitAxis].binMax.y)
                    leftObjects->push_back(m_objects->at(i));
                else
                    rightObjects->push_back(m_objects->at(i));
                break;
            }
            case 'z': {
                if(currCenter.z <= binsX[splitAxis].binMax.z)
                    leftObjects->push_back(m_objects->at(i));
                else
                    rightObjects->push_back(m_objects->at(i));
                break;
            }
        }
    }

    //Traverse down BVH and construct subtrees
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
