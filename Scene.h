#ifndef CSE168_SCENE_H_INCLUDED
#define CSE168_SCENE_H_INCLUDED

#include "Miro.h"
#include "Object.h"
#include "PointLight.h"
#include "BVH.h"
#include "Vector3.h"

class Camera;
class Image;

class Scene
{
public:
    static const bool PATH_TRACE = false;
    static const bool MAX_BOUNCE = 5;

    void addObject(Object* pObj)        {m_objects.push_back(pObj);}
    const Objects* objects() const      {return &m_objects;}

    void addLight(PointLight* pObj)     {m_lights.push_back(pObj);}
    const Lights* lights() const        {return &m_lights;}

    void preCalc();
    void openGL(Camera *cam);

    void raytraceImage(Camera *cam, Image *img);
    void pathtraceImage(Camera *cam, Image *img);
    bool trace(HitInfo& minHit, const Ray& ray,
               float tMin = 0.0f, float tMax = MIRO_TMAX) const;
    Vector3 traceRecurse(const Ray& ray,
              int bounce) const;
    Vector3 getRandDirection(HitInfo& hit) const;

    void setBGColor(const Vector3 & c) { backgroundColor = c;}
    const Vector3 & getBGColor() const { return backgroundColor; }
    void printStats();

protected:
    Objects m_objects;
    BVH m_bvh;
    Lights m_lights;
    Vector3 backgroundColor;
    static const int samples = 16;
    int tIntersect;
    int bIntersect;
};

extern Scene * g_scene;

#endif // CSE168_SCENE_H_INCLUDED
