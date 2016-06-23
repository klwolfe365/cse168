//
//  Specular.cpp
//  cse168
//
//  Written by Karen Wolfe
//
//
#include "Specular.h"
#include "Lambert.h"
#include "Ray.h"
#include "Scene.h"
#include <algorithm>

Specular::Specular(const Vector3 & kd, const Vector3 & ks,
                   const float p, const float rl, const float n, const float rf) :
    m_kd(kd), m_ks(ks), m_p(p), m_rl(rl), m_rf(rf)
{
    m_n = n;
}

Specular::~Specular()
{
}

Vector3
Specular::shade(const Ray& ray, const HitInfo& hit, const Scene& scene) const
{
    printf("Shading specular\n");
    Vector3 L = Vector3(0.0f, 0.0f, 0.0f);

    const Vector3 viewDir = -ray.d; // d is a unit vector

    const Lights *lightlist = scene.lights();
    Vector3 color = Vector3(m_kd);
    if (hit.material->hasTexture()) {
        Vector3 c = Vector3(hit.P);
		color = m_texture->getColor(c);
    }

    // loop over all of the lights

    HitInfo lightHitReflect = HitInfo(hit);
    HitInfo lightHitRefract = HitInfo(hit);
    lightHitReflect.hitNum = hit.hitNum;
    lightHitRefract.hitNum = hit.hitNum;

    Lights::const_iterator lightIter;
    for (lightIter = lightlist->begin(); lightIter != lightlist->end(); lightIter++)
    {
        PointLight* pLight = *lightIter;
        Ray lightRay;

        Vector3 l = pLight->position() - hit.P;
        float falloff = l.length2();
        l /= sqrt(falloff);

        /* Calculate the diffuse component - From Labertian Shading Model */
        float nDotL = dot(hit.N, l);
        Vector3 diffuseResult = pLight->color();
        diffuseResult *= color;
        L += std::max(0.0f, nDotL/falloff * pLight->wattage() / PI) * diffuseResult;

        /* Calculate the specular highlight - From Phong Shading Model */
        Vector3 h = (viewDir + l).normalize();
        float nDotH = dot(hit.N, h);
        Vector3 specResult = pLight->color();
        specResult *= m_ks;
        L += pow(std::max(0.0f, nDotH/falloff * pLight->wattage() / PI), m_p) * specResult;

        /* Calculate the specular reflectance */
        if(lightHitReflect.hitNum < 1){
            Vector3 r = reflect(hit.N, ray.d);
            lightRay.d = r.normalize();
            lightRay.o = hit.P;
            if(scene.trace(lightHitReflect, lightRay, 0.0001, r.length())){
                lightHitReflect.hitNum = lightHitReflect.hitNum+1;
                L += lightHitReflect.material->shade(lightRay, lightHitReflect, scene)*m_rl;
            }
        }

        /* Calculate the specular refractance */
        // Vector3 incident = viewDir - hit.P;

        if(lightHitRefract.hitNum < 1 && m_rf > 0.0f ){
            Vector3 t = refract(hit.N, ray.d, hit.material->n(), m_n);
            if(t != Vector3(0.0f))
            lightRay.d = t.normalize();
            else lightRay.d = t;
            lightRay.o = hit.P;
            if(scene.trace(lightHitRefract, lightRay, 0.0001, t.length())){
                lightHitRefract.hitNum = lightHitRefract.hitNum+1;
                    L+=lightHitRefract.material->shade(lightRay, lightHitRefract, scene)*m_rf;
            } else {
                L+=scene.getBGColor()*m_rf;
            }
        }
    }

    return L;
}

Vector3 Specular::reflect(const Vector3 & normal, const Vector3 & incident) const{
    const double cosI = dot(normal, incident);
    return incident - (2*cosI*normal);
}

Vector3 Specular::refract(const Vector3 & normal, const Vector3 & incident,
                double n1, double n2) const{
    double n;
    double cosI = dot(normal, incident);
    if(cosI < 0.0f)
        n = n1/n2;
    else
        n = n2/n1;
    double sinT2 = n * n * (1.0 - (cosI * cosI));

    if(sinT2 > 1.0){
        return Vector3(0.0f);
    }

    return (n * incident) - ((n + sqrt(1.0 - sinT2)) * normal);
}
