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
    // printf("\t...box dimensions %f %f %f.... \n", xwidth, ywidth, zwidth);
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
    float total = lCost + rCost;
    // printf("\tLeft: %d Right: %d Cost: %f\n", lNum, rNum, total);
    return total;
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
    Vector3 min;
    Vector3 max;

    printf("\n\nObjects number total %lu", m_objects->size());

    //Loop to set up bounding box widths
    for(int i = 0; i < m_objects->size(); i++){
        Vector3 tempMin = m_objects->at(i)->getMin();
        Vector3 tempMax = m_objects->at(i)->getMax();
        Vector3 center = m_objects->at(i)->getCenter();

        printf("\nCenter of object %d: %f %f %f\n", i, center.x, center.y, center.z);

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
        if(center.x < min.x)
            min.x = center.x;
        if(center.y < min.y)
            min.y = center.y;
        if(center.z < min.z)
            min.z = center.z;

        if(center.x > max.x)
            max.x = center.x;
        if(center.y > max.y)
            max.y = center.y;
        if(center.z > max.z)
            max.z = center.z;
    }

    // Create bounding box for this BVH node, set leaf status
    bbox = BoundingBox(boxMin, boxMax);
    // printf("Bounding box size MIN %f %f %f MAX %f %f %f\n", boxMin.x, boxMin.y, boxMin.z, boxMax.x, boxMax.y, boxMax.z);
    if(m_objects->size() <= NUM_LEAF_NODES){
        printf("LEAF NODE\n");
        isLeaf = true;
        return;
    }

    //Bucket sort based separation of objects into children nodes
    const int numPartitions = 4;
    float segmentWidthX = (max.x-min.x)/numPartitions;
    float segmentWidthY = (max.y-min.y)/numPartitions;
    float segmentWidthZ = (max.z-min.z)/numPartitions;
    bin binsX[numPartitions];
    bin binsY[numPartitions];
    bin binsZ[numPartitions];

    //Set up bins using partition segment widths in x/y/z planes
    for(int i = 0; i < numPartitions; i++){
        binsX[i].numObjects = 0;
        binsX[i].binMin = min;
        binsX[i].binMax = max;
        binsX[i].binMin.x = min.x + (i*segmentWidthX); //bin starting point
        binsX[i].binMax.x = min.x + ((i+1)*segmentWidthX); //bin ending point

        binsY[i].numObjects = 0;
        binsY[i].binMin = min;
        binsY[i].binMax = max;
        binsY[i].binMin.y = min.y + (i*segmentWidthY); //bin starting point
        binsY[i].binMax.y = min.y + ((i+1)*segmentWidthY); //bin ending point

        binsZ[i].numObjects = 0;
        binsZ[i].binMin = min;
        binsZ[i].binMax = max;
        binsZ[i].binMin.z = min.z + (i*segmentWidthZ); //bin starting point
        binsZ[i].binMax.z = min.z + ((i+1)*segmentWidthZ); //bin ending point

    }
    //Populate bins
    bool hasX = false;
    bool hasY = false;
    bool hasZ = false;
    for(int i = 0; i < m_objects->size(); i++){
        // printf("\nGetting object %d....", i);
        Vector3 center = m_objects->at(i)->getCenter();
        // printf("\nObject %d: ", i);
        hasX = hasY = hasZ = false;
        for(int p = 0; p < numPartitions; p++){
            // printf("Partition %d... ", p);
            if(!hasX && center.x <= binsX[p].binMax.x){
                // printf("X SORTED... ");
                binsX[p].numObjects++;
                hasX = true;
            }
            if(!hasY && center.y <= binsY[p].binMax.y){
                // printf("Y SORTED... ");
                binsY[p].numObjects++;
                hasY = true;
            }
            if(!hasZ && center.z <= binsZ[p].binMax.z){
                // printf("Z SORTED... ");
                binsZ[p].numObjects++;
                hasZ = true;
            }
        }
    }
    // printf("\nOutside of loop...\n");

    //Calculate costs to pick ideal splitting plane
    // printf("splitting at plane 0...\n");
    float xCost = getSplitPlaneCost(binsX, numPartitions, 0);
    float yCost = getSplitPlaneCost(binsY, numPartitions, 0);
    float zCost = getSplitPlaneCost(binsZ, numPartitions, 0);
    // printf("Split planes base case...\n");
    int splitX = 0;
    int splitY = 0;
    int splitZ = 0;
    float currCost = 0.0f;
    for(int i = 1; i < numPartitions; i++){
        // printf("splitting at plane %d...\n", i);
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
    // printf("Getting the optimal splitting plane...\n");
    int splitPlane = splitX;
    int splitAxis = 0;
    float minCost = fmin(xCost, fmin(yCost, zCost));
    if(minCost == yCost){
        splitPlane = splitY;
        splitAxis = 1;
    } else if(minCost == zCost){
        splitPlane = splitZ;
        splitAxis = 2;
    }

    //Divide objects along splitting plane based off center
    // printf("Getting ready to add objects for split axis %d, plane %d...\n", splitAxis, splitPlane);
    Objects * leftObjects = new Objects();
    Objects * rightObjects  = new Objects();
    for(int i = 0; i < m_objects->size(); i++){
        // printf("Adding object %d...\n", i);
        Vector3 currCenter = m_objects->at(i)->getCenter();
        // printf("\tGot center %f %f %f\n", currCenter.x, currCenter.y, currCenter.z);
        switch (splitAxis) {
            case 0: {
                // printf("Plane x ");
                if(currCenter.x <= binsX[splitPlane].binMax.x){
                    // printf(" pushback left ");
                    leftObjects->push_back(m_objects->at(i));
                }
                else{
                    // printf(" pushback right ");
                    rightObjects->push_back(m_objects->at(i));
                }
                // printf("success\n");
                break;
            }
            case 1: {
                // printf("Plane y ");
                if(currCenter.y <= binsY[splitPlane].binMax.y){
                    // printf(" pushback left ");
                    leftObjects->push_back(m_objects->at(i));
                }
                else{
                    // printf(" pushback right ");
                    rightObjects->push_back(m_objects->at(i));
                }
                    // printf("success\n");
                break;
            }
            case 2: {
                // printf("Plane z ");
                if(currCenter.z <= binsZ[splitPlane].binMax.z){
                    // printf(" pushback left ");
                    leftObjects->push_back(m_objects->at(i));
                }
                else{
                    // printf(" pushback right ");
                    rightObjects->push_back(m_objects->at(i));
                }
                    // printf("success\n");
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
