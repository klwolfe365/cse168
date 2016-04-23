#include "Specular.h"
#include "Ray.h"
#include "Scene.h"
#include <algorithm>

Specular::Specular(const Vector3 & kd, const Vector3 & ks, const float n) :
    m_kd(kd), m_ks(ks), m_n(n)
{

}

Specular::~Specular()
{
}

Vector3
Specular::shade(const Ray& ray, const HitInfo& hit, const Scene& scene) const
{
    Ray lightRay;
    Vector3 L = Vector3(0.0f, 0.0f, 0.0f);

    const Vector3 viewDir = -ray.d; // d is a unit vector

    const Lights *lightlist = scene.lights();

    // loop over all of the lights
    Lights::const_iterator lightIter;
    for (lightIter = lightlist->begin(); lightIter != lightlist->end(); lightIter++)
    {
        PointLight* pLight = *lightIter;

        Vector3 l = pLight->position() - hit.P;
        float falloff = l.length2();
        l /= sqrt(falloff);

        /* Calculate the diffuse component - From Labertian Shading Model */
        float nDotL = dot(hit.N, l);
        Vector3 diffuseResult = pLight->color();
        diffuseResult *= m_kd;

        L += std::max(0.0f, nDotL/falloff * pLight->wattage() / PI) * diffuseResult;

        /* Calculate the specular component - From Phond Shading Model */
        Vector3 h = (viewDir + l).normalize();
        float nDotH = dot(hit.N, h);
        Vector3 specResult = pLight->color();
        specResult *= m_ks;
        L += pow(std::max(0.0f, nDotH/falloff * pLight->wattage() /PI), m_n) * specResult;
    }

    // add the ambient component
    // L += m_ka;

    return L;
}
