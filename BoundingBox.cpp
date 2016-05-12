#include "BoundingBox.h"

BoundingBox::BoundingBox(){}

BoundingBox::BoundingBox(Vector3 min, Vector3 max){
    this->min = min;
    this->max = max;
}

BoundingBox::~BoundingBox(){}

bool BoundingBox::intersect(HitInfo& minHit, const Ray& ray,
        float tMin, float tMax) const {
    //Calculate min and max slabs of bounding box
    float minX, maxX, minY, maxY, minZ, maxZ = 0.0f;
    float minVal, maxVal;

    //Calculate min and max values for x/y/z planes
    if(ray.d.x != 0){
        float tx1 = (min.x - ray.o.x)/ray.d.x;
        float tx2 = (max.x - ray.o.x)/ray.d.x;
        minX = fmin(tx1, tx2);
        maxX = fmax(tx1, tx2);
    }

    if(ray.d.y != 0){
        float ty1 = (min.y - ray.o.y)/ray.d.y;
        float ty2 = (max.y - ray.o.y)/ray.d.y;
        minY = fmin(ty1, ty2);
        maxY = fmax(ty1, ty2);
    }

    if(ray.d.z != 0){
        float tz1 = (min.z - ray.o.z)/ray.d.z;
        float tz2 = (max.z - ray.o.z)/ray.d.z;
        minZ = fmin(tz1, tz2);
        maxZ = fmax(tz1, tz2);
    }

    minVal = fmax(minX, fmax(minY, minZ));
    maxVal = fmin(maxX, fmin(maxY, maxZ));
    minHit.t = minVal;
    return minVal < maxVal; // maxVal >= fmax(minVal, 0);
}

void BoundingBox::print() {
	std::cout << "BoxMin = " << min << " BoxMax = " << max << std::endl;
}

//Helper method to visualize bounding boxes, from cse169 starter code
void drawWireBox(float xmin,float ymin,float zmin,float xmax,float ymax,float zmax){
    glPushMatrix();
		glTranslatef(0.5f*(xmin+xmax),0.5f*(ymin+ymax),0.5f*(zmin+zmax));
		glScalef(xmax-xmin,ymax-ymin,zmax-zmin);
		glColor4f(1.0f,1.0f,1.0f,1.0f);
		glutWireCube(1.0f);
	glPopMatrix();
}

void BoundingBox::draw() {
	drawWireBox(min.x, min.y, min.z, max.x, max.y, max.z);
}
