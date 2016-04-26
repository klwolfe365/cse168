#include "Specular.h"
#include "Lambert.h"
#include "Ray.h"
#include "Scene.h"
#include <algorithm>

Specular::Specular(const Vector3 & kd, const Vector3 & ks, const Vector3 & km,
                   const float p, const float rl, const float rf) :
    m_kd(kd), m_ks(ks), m_km(km), m_p(p), m_rl(rl), m_rf(rf)
{

}

Specular::~Specular()
{
}

Vector3
Specular::shade(const Ray& ray, const HitInfo& hit, const Scene& scene) const
{

    Vector3 L = Vector3(0.0f, 0.0f, 0.0f);

    const Vector3 viewDir = -ray.d; // d is a unit vector

    const Lights *lightlist = scene.lights();

    // loop over all of the lights
    Lights::const_iterator lightIter;
    for (lightIter = lightlist->begin(); lightIter != lightlist->end(); lightIter++)
    {
        PointLight* pLight = *lightIter;
        Ray lightRay;
        HitInfo lightHitReflect;
        HitInfo lightHitRefract;
        lightHitReflect.hitNum = hit.hitNum + 1;
        lightHitRefract.hitNum = hit.hitNum + 1;

        Vector3 l = pLight->position() - hit.P;
        float falloff = l.length2();
        l /= sqrt(falloff);

        /* Calculate the diffuse component - From Labertian Shading Model */
        float nDotL = dot(hit.N, l);
        Vector3 diffuseResult = pLight->color();
        diffuseResult *= m_kd;
        L += std::max(0.0f, nDotL/falloff * pLight->wattage() / PI) * diffuseResult;

        /* Calculate the specular highlight - From Phong Shading Model */
        Vector3 h = (viewDir + l).normalize();
        float nDotH = dot(hit.N, h);
        Vector3 specResult = pLight->color();
        specResult *= m_ks;
        L += pow(std::max(0.0f, nDotH/falloff * pLight->wattage() / PI), m_p) * specResult;

        /* Calculate the specular reflectance */
        Vector3 r = reflect(hit.N, ray.d);
        lightRay.d = r;
        lightRay.o = hit.P;
        if(lightHitReflect.hitNum < MAX_BOUNCE){
            if(scene.trace(lightHitReflect, lightRay, 0.0001, r.length()))
                L+=lightHitReflect.material->shade(lightRay, lightHitReflect, scene)*m_rl;
        }
        
        /* Calculate the specular refractance */
        Vector3 t = refract(hit.N, viewDir, hit.material->n(), m_rf);
        lightRay.d = t.normalize();
        lightRay.o = hit.P;
        // if(lightHitRefract.hitNum < MAX_BOUNCE)
        //     L += scene.trace(lightHitRefract, lightRay, 0.0001, t.length()) * m_rf;

        // Vector3 rt = Vector3(1.0, 1.0, 1.0) - r - t;
    }

    // add the ambient component
    // L += m_ka;

    return L;
}

Vector3 Specular::reflect(const Vector3 & normal, const Vector3 & incident) const{
    const double cosI = dot(normal, incident);
    return incident - (2*cosI*normal);
}

Vector3 Specular::refract(const Vector3 & normal, const Vector3 & incident,
                double n1, double n2) const{
    const double n = n1/n2;
    const double cosI = dot(normal, incident);
    const double sinT2 = n * n * (1.0 - (cosI * cosI));

    if(sinT2 > 1.0)
        return Vector3();

    return (n * incident) - ((n + sqrt(1.0 - sinT2)) * normal);
}
