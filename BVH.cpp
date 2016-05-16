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
    Vector3 boxMin = m_objects->at(0)->getMin();
    Vector3 boxMax = m_objects->at(0)->getMax();;
    Vector3 min = m_objects->at(0)->getCenter();;
    Vector3 max = m_objects->at(0)->getCenter();;

    //Loop to set up bounding box widths
    for(int i = 0; i < m_objects->size(); i++){
        Vector3 tempMin = m_objects->at(i)->getMin();
        Vector3 tempMax = m_objects->at(i)->getMax();
        Vector3 center = m_objects->at(i)->getCenter();
        // Setting the overall widths for the current bounding box
        if(tempMin.x < boxMin.x)
            boxMin.x = tempMin.x;
        if(tempMin.y < boxMin.y)
            boxMin.y = tempMin.y;
        if(tempMin.z < boxMin.z)
            boxMin.z = tempMin.z;

        if(tempMax.x > boxMax.x)
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
    if(m_objects->size() <= NUM_LEAF_NODES){
        isLeaf = true;
        return;
    }

    //Bucket sort based separation of objects into children nodes
    const int numPartitions = 5;
    float segWidthX = (max.x - min.x)/numPartitions;
    float segWidthY = (max.y - min.y)/numPartitions;
    float segWidthZ = (max.z - min.z)/numPartitions;
    bin binsX[numPartitions];
    bin binsY[numPartitions];
    bin binsZ[numPartitions];
    //Set up bins using partition segment widths in x/y/z planes
    for(int i = 0; i < numPartitions; i++){
        binsX[i].numObjects = 0;
        binsX[i].binMin = min;
        binsX[i].binMax = max;
        binsX[i].binMin.x = min.x + (i*segWidthX); //bin starting point
        binsX[i].binMax.x = min.x + ((i+1)*segWidthX); //bin ending point

        binsY[i].numObjects = 0;
        binsY[i].binMin = min;
        binsY[i].binMax = max;
        binsY[i].binMin.y = min.y + (i*segWidthY); //bin starting point
        binsY[i].binMax.y = min.y + ((i+1)*segWidthY); //bin ending point

        binsZ[i].numObjects = 0;
        binsZ[i].binMin = min;
        binsZ[i].binMax = max;
        binsZ[i].binMin.z = min.z + (i*segWidthZ); //bin starting point
        binsZ[i].binMax.z = min.z + ((i+1)*segWidthZ); //bin ending point

    }
    //Populate bins
    bool hasX = false;
    bool hasY = false;
    bool hasZ = false;
    for(int i = 0; i < m_objects->size(); i++){
        Vector3 center = m_objects->at(i)->getCenter();
        hasX = hasY = hasZ = false;
        for(int p = 0; p < numPartitions; p++){
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
    int planeNum = splitX;
    int axis = 0;
    float minCost = fmin(xCost, fmin(yCost, zCost));
    if(minCost == yCost){
        planeNum = splitY;
        axis = 1;
    } else if(minCost == zCost){
        planeNum = splitZ;
        axis = 2;
    }

    //Divide objects along splitting plane based off center
    Objects * leftObjects = new Objects();
    Objects * rightObjects = new Objects();
    for(int i = 0; i < m_objects->size(); i++){
        Vector3 currCenter = m_objects->at(i)->getCenter();
        switch (axis) {
            case 0: {
                if(currCenter.x <= binsX[splitX].binMax.x){
                    leftObjects->push_back(m_objects->at(i));
                }
                else{
                    rightObjects->push_back(m_objects->at(i));
                }
                break;
            }
            case 1: {
                if(currCenter.y <= binsY[splitY].binMax.y){
                    leftObjects->push_back(m_objects->at(i));
                }
                else{
                    rightObjects->push_back(m_objects->at(i));
                }
                break;
            }
            case 2: {
                if(currCenter.z <= binsZ[splitZ].binMax.z){
                    leftObjects->push_back(m_objects->at(i));
                }
                else{
                    rightObjects->push_back(m_objects->at(i));
                }
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
    // printf("bvh intersect\n");
    bool hit = false;
    if(isLeaf){
        // printf("Intersect leaf\n");
        HitInfo tempMinHit;
        tempMinHit.triHit = minHit.triHit;
        tempMinHit.boxHit = minHit.boxHit;
        minHit.t = MIRO_TMAX;
        int hitCount = 0;
        for (size_t i = 0; i < m_objects->size(); ++i)
        {
            if ((*m_objects)[i]->intersect(tempMinHit, ray, tMin, tMax))
            {
                hit = true;
                hitCount++;
                if (tempMinHit.t < minHit.t){
                    // tempMinHit.hitNum = minHit.hitNum;
                    minHit = tempMinHit;
                    minHit.material = tempMinHit.material;

                }
            }
        }
        // printf("Triangle hit %d vs %d\n", minHit.triHit, hitCount);
        minHit.triHit = hitCount;
    } else {
        HitInfo lMinHit;
        HitInfo rMinHit;
        HitInfo lMin;
        HitInfo rMin;
        bool lHit = left_child->getBoundingBox().intersect(lMin, ray, tMin, tMax);
        bool rHit = right_child->getBoundingBox().intersect(rMin, ray, tMin, tMax);
        if(lHit && rHit){
            left_child->getBoundingBox().incHitNum();
            right_child->getBoundingBox().incHitNum();
            // printf("Left and right child bbox hit...\n");
            lHit = left_child->intersect(lMinHit, ray, tMin, tMax);
            rHit = right_child->intersect(rMinHit, ray, tMin, tMax);
            if(lHit){
                hit = true;
                minHit = lMinHit;
                minHit.material = lMinHit.material;
            }

            if(rHit){
                if(lHit){
                    //Hit both boxes, check for which one is closer
                    if(rMinHit.t < lMinHit.t){
                        hit = true;
                        minHit = rMinHit;
                        minHit.material = rMinHit.material;
                    }
                }
                //Only hit right box
                else {
                    hit = true;
                    minHit = rMinHit;
                    minHit.material = rMinHit.material;
                }
                // hit = true;
            }
            minHit.boxHit = lMinHit.boxHit + rMinHit.boxHit + 1;
        }
        else if (lHit){
            left_child->getBoundingBox().incHitNum();//.hitNum++;
            // printf("Hit left bbox child only\n");
            if(left_child->intersect(lMinHit, ray, tMin, tMax)){
                hit = true;
                minHit = lMinHit;
                minHit.material = lMinHit.material;
            }
            minHit.boxHit = lMinHit.boxHit + 1;
        }
        else if(rHit){
            right_child->getBoundingBox().incHitNum();//.hitNum++;
            // printf("Hit right bbox child only\n");
            if(right_child->intersect(rMinHit, ray, tMin, tMax)){
                hit = true;
                minHit = rMinHit;
                minHit.material = rMinHit.material;
            }
            minHit.boxHit = rMinHit.boxHit + 1;
        } else {
            hit = false;
        }
    }
    // printf("Returning\n");
    return hit;
}

int BVH::getLeafNum(){
    if(isLeaf)
    return 1;
    return left_child->getLeafNum() + right_child->getLeafNum();
}

int BVH::getNodeNum(){
    if(isLeaf)
    return 1;
    return left_child->getNodeNum() + right_child->getNodeNum() + 1;
}

int BVH::getBoundingBoxHits(){
    if(isLeaf)
        return bbox.hitNum;
    return left_child->getBoundingBoxHits() + right_child->getBoundingBoxHits() + bbox.hitNum;
}

int BVH::getTriangleHits(){
    if(isLeaf)
        return bbox.triHit;
    return left_child->getTriangleHits() + right_child->getTriangleHits();
}
