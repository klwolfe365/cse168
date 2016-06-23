//
//  PencilShader.cpp
//  cse168
//
//  Written by Karen Wolfe
//
//
#include "PencilShader.h"
#include "Ray.h"
#include "Scene.h"
#include <algorithm>

#define SAMPLES 10
#define RAD_H 0.00125

PencilShader::PencilShader(const Vector3 & kd, const Vector3 & ka) :
    Lambert(kd, ka)//m_kd(kd), m_ka(ka)
{
}

PencilShader::~PencilShader()
{
}


Vector3
PencilShader::shade(const Ray& ray, const HitInfo& hit, const Scene& scene) const
{

    int m = 0;
    int totalRays = 0;
    float step = (float)RAD_H/SAMPLES;
    std::vector<Vector3> normals;
    std::vector<HitInfo> hits;

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
                    normals.push_back(Vector3(h.N));
                    hits.push_back(h);

                }
            } else m++;
        }
    }

    //Hit other geometry, outline edge
    if(m > 0){
        return Vector3(0.0f);
    }

    float gradient = 0.0;
    //Check for creases or silhouettes
    for(int i = 0; i <= normals.size(); i++){
        gradient += (dot(normals[i], hit.N))/normals.size();
        if(gradient < 0.01)
            return Vector3(0.0f);
    }

    Vector3 L = Vector3(0.0f, 0.0f, 0.0f);
    const Lights *lightlist = scene.lights();
    Ray shadow;
    shadow.o = hit.P - ray.o;

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
        shadow.d = l;
        float nDotL = dot(hit.N, l);
        //Map into color location
        L += getTextureColor(nDotL, hit);
    }
    return L;
}

Vector3 PencilShader::getTextureColor(float nl, /*const Ray& ray,*/ const HitInfo& hit/*, const Scene& scene*/) const {

    Texture * bg = textures[0];
    Texture * t;
    int hatchIndex = 0;
    int numTextures = textures.size();
    float step = 1.0/numTextures;
    int c = 0;
    for(int i = numTextures - 1 ; i >= 0; i--){
        float density = step*(1+c);
        if(nl < density){
            hatchIndex = i;
            break;
        }
        c++;
    }
    Vector3 color = Vector3(0.0f);
    // Weight each texture according to density of the hatch index
    for(int i = 0; i <= hatchIndex; i++){
        t = textures[i];
        if(t == NULL)
            return color;
        int w = t->m_image.width();
        int h = t->m_image.height();
        int x = hit.u;
        int y = hit.v;
        if(x >= w && w != 0)
            x = x % w;
        if(y >= h && h != 0)
            y = y % h;
        int index = (x + w*y)*3;
        float r = (t->m_rawData[index]/255.0)/(hatchIndex+1.0);
        float g = (t->m_rawData[index + 1]/255.0)/(hatchIndex+1.0);
        float b = (t->m_rawData[index + 2]/255.0)/(hatchIndex+1.0);
        color += Vector3(r,g,b);
    }
    return color;
}


void PencilShader::loadTextures(std::vector<const char * > filenames){
    for(int i = 0; i < filenames.size(); i++){
        textures.push_back(new Texture());
        textures[i]->loadTexture(filenames[i]);
    }
}

void PencilShader::loadTexture(const char * filename){
    Texture * t = new Texture();
    t->loadTexture(filename);
    textures.push_back(t);
}
