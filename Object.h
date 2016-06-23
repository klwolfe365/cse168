//
//  Object.h
//  cse168
//
//  Provided in CSE168 starter code
//  Modified by Karen Wolfe to add object boundaries, center, and id
//
//
#ifndef CSE168_OBJECT_H_INCLUDED
#define CSE168_OBJECT_H_INCLUDED

#include <vector>
#include "Miro.h"
#include "Material.h"

class Object
{
public:
    Object() {}
    virtual ~Object() {}

    void setMaterial(const Material* m) {m_material = m;}

    virtual void renderGL() {}
    virtual void preCalc() {}

    virtual bool intersect(HitInfo& result, const Ray& ray,
                           float tMin = 0.0f, float tMax = MIRO_TMAX) = 0;

    virtual Vector3 getCenter() { return Vector3(0.0, 0.0, 0.0); }
    virtual Vector3 getMin()    { return Vector3(0.0, 0.0, 0.0); }
    virtual Vector3 getMax()    { return Vector3(0.0, 0.0, 0.0); }

    // virtual void setUVCoordinates();

    int objId;
    float u;
    float v;
protected:
    const Material* m_material;

};

typedef std::vector<Object*> Objects;

#endif // CSE168_OBJECT_H_INCLUDED
