//
//  CellShading.cpp
//  cse168
//
//  Written by Karen Wolfe
//
//

#include "CellShading.h"
#include "Ray.h"
#include "Scene.h"
#include <algorithm>

CellShading::CellShading(const Vector3 & kd, const Vector3 & ka,
    float dr, float mr, float lr, float di, float mi, float li) :
    Lambert(kd, ka)
{
    if(dr > 1.0 || dr < 0.0)
        printf("Warning, darkness band out of allowed range");
    if(mr > 1.0 || mr < 0.0)
        printf("Warning, mid band out of allowed range");
    if(mr < dr)
        printf("Warning, mid color range should be greater than the darkness range");
    if(lr > 1.0 || lr < 0.0)
        printf("Warning, mid band out of allowed range");
    if(lr < dr || lr < mr)
        printf("Warning, mid color range should be greater than the darker ranges");

    darkRange = dr;
    midRange = mr;
    lightRange = lr;

    darkIntensity = di;
    midIntensity = mi;
    lightIntensity = li;
}

CellShading::~CellShading()
{
}


Vector3
CellShading::shade(const Ray& ray, const HitInfo& hit, const Scene& scene) const
{
    Vector3 L = Vector3(0.0f, 0.0f, 0.0f);

    const Vector3 viewDir = -ray.d; // d is a unit vector

    const Lights *lightlist = scene.lights();
    Vector3 color = Vector3(m_kd);
    if (hit.material->hasTexture()) {
        Vector3 c = Vector3(hit.P);
		color = m_texture->getColor(c);
    }
    // loop over all of the lights
    Lights::const_iterator lightIter;
    for (lightIter = lightlist->begin(); lightIter != lightlist->end(); lightIter++)
    {
        PointLight* pLight = *lightIter;

        Vector3 l = pLight->position() - hit.P;

        // the inverse-squared falloff
        float falloff = l.length2();

        // normalize the light direction
        l /= sqrt(falloff);

        // get the diffuse component
        float nDotL = dot(hit.N, l);
        //Map into color location
        L += getCellColor(nDotL);
    }

    return L;
}

Vector3 CellShading::getCellColor(float nl) const {
    if(nl <= darkRange)
        return Vector3(m_kd)*darkIntensity;
    if(nl <= midRange)
        return Vector3(m_kd)*midIntensity;
    if(nl <= lightRange)
        return Vector3(m_kd)*lightIntensity;

    return Vector3(1.0f);
}
