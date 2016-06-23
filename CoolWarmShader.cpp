//
//  CoolWarmShader.cpp
//  cse168
//
//  Written by Karen Wolfe
//
//
#include "CoolWarmShader.h"
#include "Ray.h"
#include "Scene.h"
#include <algorithm>

#define SAMPLES 2
#define RAD_H 0.0025


CoolWarmShader::CoolWarmShader(const Vector3 & cool, const Vector3 & warm, bool edges) {
    m_cool = cool;
    m_warm = warm;
    m_edges = edges;
}

CoolWarmShader::~CoolWarmShader(){}
Vector3
CoolWarmShader::shade(const Ray& ray, const HitInfo& hit, const Scene& scene) const
{
    if(m_edges){
        Vector3 hitPoint = hit.P - ray.o;
        int m = 0;
        int totalRays = 0;
        float step = (float)RAD_H/SAMPLES;
        std::vector<Vector3> normals;
        for(float r = step; r <= RAD_H; r+=step){
            float theta_step = (2.0*M_PI)/(pow(2, r+2));
            for(float theta = 0; theta <= 2.0*M_PI; theta += theta_step){
                totalRays++;
                float x = r*cos(theta);
                float y = r*sin(theta);
                Ray stencilRay;
                Vector3 dir = Vector3(ray.d);
                dir.x += x;
                dir.y += y;
                stencilRay.o = ray.o;
                stencilRay.d = dir;

                HitInfo h;
                if(scene.trace(h, stencilRay)){

                    if(h.objId != hit.objId){
                        m++;
                    } else{
                        normals.push_back(h.N);
                    }
                } else m++;
            }
        }


    }


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


Vector3 CoolWarmShader::getCellColor(float nl) const {
    float kw = (1.0 + nl)/2.0;

    Vector3 c = kw*m_warm + (1-kw)*m_cool;

    return c;
}
