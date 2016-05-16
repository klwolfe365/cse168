#include "Miro.h"
#include "Scene.h"
#include "Camera.h"
#include "Image.h"
#include "Console.h"
#include "Specular.h"
#include <math.h>
#define EPSILON 0.000001

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
                // if(s == 0)
                //     ray = cam->eyeRay(i, j, img->width(), img->height());
                // else
                    ray = cam->shiftEyeRay(i, j, img->width(), img->height());
                if (trace(hitInfo, ray))
                    colorSum += traceRecurse(ray, 0) / (double)samples;
            }
            img->setPixel(i, j, colorSum);
        }
        img->drawScanline(j);
        glFinish();
        // printf("Rendering Progress: %.3f%%\r", j/float(img->height())*100.0f);
        fflush(stdout);
    }

    printf("Rendering Progress: 100.000%\n");
    debug("done Raytracing!\n");
}

bool
Scene::trace(HitInfo& minHit, const Ray& ray, float tMin, float tMax) const
{
    // const Specular * spec = dynamic_cast<const Specular *>(minHit.material);
    // if(spec != NULL && minHit.hitNum < 100)
    // printf("Reg TRACE hitnum %d... \n", minHit.hitNum);
    // minHit.hitNum++;
    return m_bvh.intersect(minHit, ray, tMin, tMax);;
}


Vector3
Scene::traceRecurse(const Ray & ray, int bounce) const {
    HitInfo hit;
    if(trace(hit, ray)){
        Vector3 c = hit.material->shade(ray, hit, *this);
        if(bounce < MAX_BOUNCE){
            Ray newRay;
            newRay.o = hit.P;
            newRay.d = getRandDirection(hit);
            return c + (0.5 * traceRecurse(newRay, bounce + 1));
        }
    } else {
        return Vector3(0.0);
    }
}
//
// Vector3
// Scene::traceRecurse(const Ray& ray, int bounce) const
// {
//     if(bounce > MAX_BOUNCE){
//         return Vector3(0.0);
//     }
//     HitInfo hit;
//     if(!trace(hit, ray)){
//         return Vector3(0.0);
//     }
//
//     float randnum = (float)(rand() / RAND_MAX); //get a random number 0-1.0f
//     // float em = hit.object->material()->emittance();
//
//     Vector3 randDir = hit.material->randomReflect(-ray.d, hit.N);
//
//     // Vector3 gathered;
//     // Lights::const_iterator lightIter;
//     // for (lightIter = m_lights.begin(); lightIter != m_lights.end(); lightIter++)
//     // {
//     //
//     //     PointLight* pLight = *lightIter;
//     //     gathered += sampleLight(hit, pLight);
//     //     // ray.d = pLight->position() - hitInfo.P;
//     //     // ray.d.normalize();
//     //     // ray.o = ray.o + ray.d * epsilon;
//     //     // if (trace(hitInfo, ray)) {
//     //     //     if (hitInfo.t > EPSILON) {
//     //     //         shadeResult = Vector3(0);
//     //     //         break;
//     //     //     }
//     //     // }
//     // }
//
//
//
//
//
//
//     // float randNum = (float)(rand()/RAND_MAX); // Get random number 0->1.0
//     // float emit = hit.material->emittance();
//     //
//     // Vector3 reflect = hit.material->shade(ray, hit, *this);
//     // printf("Reflectance %f %f %f\n", reflect.x, reflect.y, reflect.z);
//     // if(emit == 1.0 || randNum < emit){
//     //     Vector3 radiance =
//     // }
//     Ray newRay;// = getBounceRay();
//     newRay.o = hit.P;
//     newRay.d = randDir.normalized();
//
//     Vector3 brdf = hit.material->BRDF(-ray.d, hit.N, newRay.d);
//
//     float cos_theta = dot(hit.N, newRay.d);
//     // Vector3 bdrf =  * cos_theta;
//     Vector3 color = traceRecurse(newRay, bounce+1);
//     Vector3 gather = hit.material->shade(ray, hit, *this);
//     float em = 1.0f/(4.0*M_PI);
//     // float em = hit.material->
//     // const Specular * spec = dynamic_cast<const Specular *>(minHit.material);
//     // if(spec != NULL && minHit.hitNum < 100)
//     // printf("Reg TRACE hitnum %d... \n", minHit.hitNum);
//     // minHit.hitNum++;
//     // Vector3 bdrf = 2 * PI * reflect;
//     return gather + (brdf*cos*traceRecurse(newRay, bounce + 1)*(1.0 / (1.0 - em)) / ;
// }

// bool
// Scene::trace2(HitInfo& minHit, const Ray& ray, float tMin, float tMax) const
// {
//     minHit.hitNum++;
//     printf("Reg TRACE hitnum %d... \n", minHit.hitNum);
//     return m_bvh.intersect(minHit, ray, tMin, tMax);
// }
Vector3
Scene::getRandDirection(HitInfo& hitInfo) const {
    double phi = 2.0 * PI * (double)rand() / (double)RAND_MAX;
    double theta = acos(2.0*(double)rand() / RAND_MAX - 1.0);
    return Vector3(sin(theta)*cos(phi), sin(theta)*sin(phi), cos(theta));
}

void Scene::printStats(){
    printf("\n=========== Statistics ===========\n");
    printf("Number of nodes in bvh: %d\n", m_bvh.getNodeNum());
    printf("Number of leafs in bvh: %d\n", m_bvh.getLeafNum());
    printf("Number of intersections: %d\n", bIntersect);
    printf("Number of triangle hits: %d\n", tIntersect);
    printf("==================================\n");
}

Vector3 Scene::sampleLight(HitInfo& hit, PointLight* light) const{
    Vector3 lValue;
    for(int s = 0; s < samples; s++){
        Vector3 randPoint = getRandomPoint(light);
        //Check if light is visible
        Ray ray;
        ray.d = randPoint - hit.P;
        ray.d.normalize();
        ray.o = ray.o + ray.d * epsilon;
        HitInfo hitInfo;
        if (trace(hitInfo, ray)) {
            if (hitInfo.t > EPSILON) {
                lValue += Vector3(0.0f);
            }
        } else {
            lValue += hit.material->shade(ray, hit, *this)/samples;//get local illumination TODO
        }
    }
    return lValue;
}

Vector3 Scene::getRandomPoint(PointLight* light) const{
    Vector3 pos = light->position();
    HitInfo hit;
    return pos + getRandDirection(hit);
}
