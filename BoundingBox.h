#ifndef CSE168_BOUNDINGBOX_H_INCLUDED
#define CSE168_BOUNDINGBOX_H_INCLUDED

#include "Object.h"
#include "Ray.h"

class BoundingBox
{
public:

	BoundingBox();
	BoundingBox(Vector3 min, Vector3 max);
	~BoundingBox();
	bool intersect(HitInfo& minHit, const Ray& ray, float tMin, float tMax);
	void print();
	void draw();
	int hitNum;
	int triHit;

    void incHitNum() { hitNum++; }
	void incTriHit(int t) {triHit += t;}

protected:

	Vector3 min;
	Vector3 max;
};

#endif // CSE168_BOUNDINGBOX_H_INCLUDED
