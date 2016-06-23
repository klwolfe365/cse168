//
//  Scene.cpp
//  cse168
//
//  Provided in CSE168 starter code
//  Modified by Karen Wolfe
//  Added photonmapping, path tracing, and recursive tracing of rays
//
#include "Miro.h"
#include "Scene.h"
#include "Camera.h"
#include "Image.h"
#include "Console.h"
#include "Specular.h"
#include <math.h>
#define EPSILON 0.000001

struct hitpoint{
    Vector3 position;
    Vector3 normal;
    Vector3 omega;      //ray direction
    int BRDF_index;
    float x, y;         // pixel location
    Vector3 weight;
    float radius;
    int photon_count;
    Vector3 tau;        // accumulated reflected flux
};

Scene * g_scene = 0;

void
Scene::openGL(Camera *cam)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    cam->drawGL();

    // draw objects
    for (size_t i = 0; i < m_objects.size(); ++i)
        m_objects[i]->renderGL();

    glutSwapBuffers();
}

void
Scene::preCalc()
{
    Objects::iterator it;
    for (it = m_objects.begin(); it != m_objects.end(); it++)
    {
        Object* pObject = *it;
        pObject->preCalc();
    }
    Lights::iterator lit;
    for (lit = m_lights.begin(); lit != m_lights.end(); lit++)
    {
        PointLight* pLight = *lit;
        pLight->preCalc();
    }

    m_bvh.build(&m_objects);
}

void
Scene::raytraceImage(Camera *cam, Image *img)
{
    Ray ray;
    HitInfo hitInfo;
    Vector3 shadeResult;
    bIntersect = 0;
    tIntersect = 0;
    bool useShadows = false;

    // loop over all pixels in the image
    for (int j = 0; j < img->height(); ++j)
    {
        for (int i = 0; i < img->width(); ++i)
        {
            ray = cam->eyeRay(i, j, img->width(), img->height());
            hitInfo.boxHit = 0;
            hitInfo.triHit = 0;
            if (trace(hitInfo, ray))
            {
                // printf("Traced...\n");
                hitInfo.u = i;
                hitInfo.v = j;
                const Material * mat = hitInfo.material;
                shadeResult = mat->shade(ray, hitInfo, *this);
                if (useShadows) {
                    Lights::const_iterator lightIter;
                    ray.o = hitInfo.P;
                    for (lightIter = m_lights.begin(); lightIter != m_lights.end(); lightIter++)
                    {
                        PointLight* pLight = *lightIter;
                        ray.d = pLight->position() - hitInfo.P;
                        ray.d.normalize();
                        ray.o = ray.o + ray.d * epsilon;
                        if (trace(hitInfo, ray)) {
                            if (hitInfo.t > EPSILON) {
                                shadeResult = Vector3(0);
                                break;
                            }
                        }
                    }
                }
                img->setPixel(i, j, shadeResult);
            }
            // bIntersect += hitInfo.boxHit;
            tIntersect += hitInfo.triHit;
            bIntersect += hitInfo.boxHit;
        }
        img->drawScanline(j);
        glFinish();
        // printf("Rendering Progress: %.3f%%\r", j/float(img->height())*100.0f);
        fflush(stdout);
    }

    printf("Rendering Progress: 100.000%\n");
    debug("done Raytracing!\n");
    printStats();
}
////////////////////////////////////////////////////////////////////////////////
//@author Karen Wolfe
void
Scene::pathtraceImage(Camera *cam, Image *img)
{
    Ray ray;
    HitInfo hitInfo;
    Vector3 shadeResult;
    bool useShadows = false;
    // loop over all pixels in the image
    for (int j = 0; j < img->height(); ++j)
    {
        for (int i = 0; i < img->width(); ++i) {
            Vector3 colorSum = Vector3(0.0, 0.0, 0.0);

            for(int s = 0; s < samples; s++) {
                ray = cam->eyeRay(i, j, img->width(), img->height());
                if (trace(hitInfo, ray))
                    colorSum += traceRecurse(ray, 0) / (float)samples;
            }
            img->setPixel(i, j, colorSum);
        }
        img->drawScanline(j);
        glFinish();
        printf("Rendering Progress: %.3f%%\r", j/float(img->height())*100.0f);
        fflush(stdout);
    }

    printf("Rendering Progress: 100.000%\n");
    debug("done Raytracing!\n");
}

//@author Karen Wolfe
void
Scene::photonMapImage(Camera *cam, Image *img){
    Ray ray;
    HitInfo hitInfo;
    Vector3 shadeResult;
    bool useShadows = false;
    // loop over all pixels in the image
    std::vector<hitpoint *> hitpoints;

    for (int j = 0; j < img->height(); ++j)
    {
        for (int i = 0; i < img->width(); ++i) {
            hitpoint* hp = new hitpoint;
            ray = cam->eyeRay(i, j, img->width(), img->height());
            Vector3 colorSum = hitInfo.material->shade(ray, hitInfo, *this);
            //Store info about hit location for path tracing
            hp->x = i;
            hp->y = j;
            hp->position = hitInfo.P;
            hp->normal = hitInfo.N;
            hp->omega = ray.d;
            hp->weight = colorSum;
            hp->radius = 10.0;
            hp->photon_count = 100;
            hp->tau = 0.0;
            hitpoints.push_back(hp);
            img->setPixel(i, j, colorSum);
        }
        img->drawScanline(j);
        glFinish();
        printf("Rendering Progress: %.3f%%\r", j/float(img->height())*100.0f);
        fflush(stdout);
    }

    //BEGIN PHOTON MAPPING - initialize photon mapj
    //loop through the lights to construct an initial photon mafp
    for(hitpoint* h : hitpoints){
        float density = h->photon_count/(M_PI*h->radius*h->radius);

    }
    printf("Rendering Progress: 100.000%\n");
    debug("done Raytracing!\n");

}

//@author Karen Wolfe
bool
Scene::trace(HitInfo& minHit, const Ray& ray, float tMin, float tMax) const
{
    return m_bvh.intersect(minHit, ray, tMin, tMax);
}

//@author Karen Wolfe
Vector3
Scene::traceRecurse(const Ray & ray, int bounce) const {
    HitInfo hit;
    if(trace(hit, ray)){
        Vector3 c = hit.material->shade(ray, hit, *this);
        if(bounce < MAX_BOUNCE){
            Ray newRay;
            newRay.o = hit.P;
            newRay.d = getRandDirection(hit);
            float em = hit.material->getEmittance(ray, hit, *this);
            return c + (0.5 * traceRecurse(newRay, bounce + 1));
        }
        return c;
    } else {
        return Vector3(0.0);
    }
}

//@author Karen Wolfe
Vector3
Scene::getRandDirection(HitInfo& hitInfo) const {
    double phi = 2.0 * PI * (double)rand() / (double)RAND_MAX;
    double theta = acos(2.0*(double)rand() / RAND_MAX - 1.0);
    return Vector3(sin(theta)*cos(phi), sin(theta)*sin(phi), cos(theta));
}

//@author Karen Wolfe
void Scene::printStats(){
    printf("\n=========== Statistics ===========\n");
    printf("Number of nodes in bvh: %d\n", m_bvh.getNodeNum());
    printf("Number of leafs in bvh: %d\n", m_bvh.getLeafNum());
    printf("Number of intersections: %d\n", bIntersect);
    printf("Number of triangle hits: %d\n", tIntersect);
    printf("==================================\n");
}
