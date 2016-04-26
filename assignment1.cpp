#include "assignment1.h"
#include <math.h>
#include "Miro.h"
#include "Scene.h"
#include "Camera.h"
#include "Image.h"
#include "Console.h"

#include "PointLight.h"
#include "TriangleMesh.h"
#include "Triangle.h"
#include "Lambert.h"
#include "Specular.h"
#include "Sphere.h"

void
makeBunnyScene()
{
    g_camera = new Camera;
    g_scene = new Scene;
    g_image = new Image;

    g_image->resize(128, 128);

    // set up the camera
    g_camera->setBGColor(Vector3(1.0f, 1.0f, 1.0f));
    g_camera->setEye(Vector3(-2, 3, 5));
    g_camera->setLookAt(Vector3(-.5, 1, 0));
    g_camera->setUp(Vector3(0, 1, 0));
    g_camera->setFOV(45);

    // create and place a point light source
    PointLight * light = new PointLight;
    light->setPosition(Vector3(-3, 15, 3));
    light->setColor(Vector3(1, 1, 1));
    light->setWattage(500);
    g_scene->addLight(light);

    Material* matte = new Specular(Vector3(1.0f, 1.0f, 1.0f), Vector3(1.0f), Vector3(0.0f), 25.0f, 1.0f, 0.0f);
    Specular* shiny = new Specular(Vector3(0.8f, 0.8f, 0.8f), Vector3(1.0f), Vector3(0.0f), 25.0f, 0.1f, 0.0f);

    TriangleMesh * bunny = new TriangleMesh;
    bunny->load("bunny.obj");

    // create all the triangles in the bunny mesh and add to the scene
    for (int i = 0; i < bunny->numTris(); ++i)
    {
        Triangle* t = new Triangle;
        t->setIndex(i);
        t->setMesh(bunny);
        t->setMaterial(shiny);
        g_scene->addObject(t);
    }

    // create the floor triangle
    TriangleMesh * floor = new TriangleMesh;
    floor->createSingleTriangle();
    floor->setV1(Vector3(  0, 0,  10));
    floor->setV2(Vector3( 10, 0, -10));
    floor->setV3(Vector3(-10, 0, -10));
    floor->setN1(Vector3(0, 1, 0));
    floor->setN2(Vector3(0, 1, 0));
    floor->setN3(Vector3(0, 1, 0));

    Triangle* t = new Triangle;
    t->setIndex(0);
    t->setMesh(floor);
    t->setMaterial(matte);
    g_scene->addObject(t);

    // let objects do pre-calculations if needed
    g_scene->preCalc();
}

void
makeSphereScene()
{
    g_camera = new Camera;
    g_scene = new Scene;
    g_image = new Image;

    g_image->resize(512, 512);

    // set up the camera
    g_camera->setBGColor(Vector3(1.0f, 1.0f, 1.0f));
    g_camera->setEye(Vector3(-5, 2, 3));
    g_camera->setLookAt(Vector3(0, 0, 0));
    g_camera->setUp(Vector3(0, 1, 0));
    g_camera->setFOV(45);

    // create and place a point light source
    PointLight * light = new PointLight;
    light->setPosition(Vector3(-3, 15, 3));
    light->setColor(Vector3(1, 1, 1));
    light->setWattage(300);
    g_scene->addLight(light);

    // create a spiral of spheres
    // Material* mat = new Lambert(Vector3(1.0f, 0.0f, 0.0f));
    Material* matte = new Lambert(Vector3(1.0f, 1.0f, 1.0f));
    Material* shiny = new Specular(Vector3(1.0f), Vector3(0.5f), Vector3(1.0), 0.2f, 0.2f, 0.0f);
    TriangleMesh * sphere = new TriangleMesh;
    sphere->load("sphere.obj");

    // create all the triangles in the bunny mesh and add to the scene
    for (int i = 0; i < sphere->numTris(); ++i)
    {
        Triangle* t = new Triangle;
        t->setIndex(i);
        t->setMesh(sphere);
        t->setMaterial(shiny);
        g_scene->addObject(t);
    }

    // create the floor triangle
    TriangleMesh * floor = new TriangleMesh;
    floor->createSingleTriangle();
    floor->setV1(Vector3(  0, -2,  10));
    floor->setV2(Vector3( 10, -2, -10));
    floor->setV3(Vector3(-10, -2, -10));
    floor->setN1(Vector3(0, 1, 0));
    floor->setN2(Vector3(0, 1, 0));
    floor->setN3(Vector3(0, 1, 0));

    Triangle* t = new Triangle;
    t->setIndex(0);
    t->setMesh(floor);
    t->setMaterial(matte);
    g_scene->addObject(t);

    // let objects do pre-calculations if needed
    g_scene->preCalc();
}

void
makeTeapotScene()
{
    g_camera = new Camera;
    g_scene = new Scene;
    g_image = new Image;

    g_image->resize(128, 128);

    // set up the camera
    g_camera->setBGColor(Vector3(0.0f, 0.0f, 0.0f));
    g_camera->setEye(Vector3(-2, 3, 5));
    g_camera->setLookAt(Vector3(-.5, 1, 0));
    g_camera->setUp(Vector3(0, 1, 0));
    g_camera->setFOV(45);

    // create and place a point light source
    PointLight * light = new PointLight;
    light->setPosition(Vector3(-1, 10, 10));
    light->setColor(Vector3(1.0, 1.0, 1.0));
    light->setWattage(500);
    g_scene->addLight(light);
    // light = new PointLight;
    // light->setPosition(Vector3(10,0,-3));
    // light->setColor(Vector3(1.0, 1.0, 1.0));
    // light->setWattage(5);
    // g_scene->addLight(light);
    Material* matte = new Lambert(Vector3(1.0, 0.0, 0.0));
    Specular* shiny = new Specular(Vector3(0.9f, 0.9f, 0.90f), Vector3(0.9f, 0.9f, 0.90f), Vector3(0.9f, 0.9f, 0.90f), 10.0f, 0.1f, 0.0f);

    TriangleMesh * teapot = new TriangleMesh;
    teapot->load("teapot.obj");

    // create all the triangles in the bunny mesh and add to the scene
    for (int i = 0; i < teapot->numTris(); ++i)
    {
        Triangle* t = new Triangle;
        t->setIndex(i);
        t->setMesh(teapot);
        t->setMaterial(shiny);
        g_scene->addObject(t);
    }

    // create the floor triangle
    TriangleMesh * floor = new TriangleMesh;
    floor->createSingleTriangle();
    floor->setV1(Vector3(  0, .3,  3));
    floor->setV2(Vector3( 10, .3, -3));
    floor->setV3(Vector3(-10, .3, -3));
    floor->setN1(Vector3(0, 1, 0));
    floor->setN2(Vector3(0, 1, 0));
    floor->setN3(Vector3(0, 1, 0));

    Triangle* t = new Triangle;
    t->setIndex(0);
    t->setMesh(floor);
    t->setMaterial(matte);
    g_scene->addObject(t);

    // let objects do pre-calculations if needed
    g_scene->preCalc();
}

void makeTwoSphereScene(){
    g_camera = new Camera;
    g_scene = new Scene;
    g_image = new Image;

    g_image->resize(512, 512);

    // set up the camera
    g_camera->setBGColor(Vector3(1.0f, 1.0f, 1.0f));
    g_camera->setEye(Vector3(0, 2, -2));
    g_camera->setLookAt(Vector3(0, 0, 0));
    g_camera->setUp(Vector3(0, 1, 0));
    g_camera->setFOV(45);

    // create and place a point light source
    PointLight * light = new PointLight;
    light->setPosition(Vector3(-3, 15, 3));
    light->setColor(Vector3(1, 1, 1));
    light->setWattage(800);
    g_scene->addLight(light);

    // create a spiral of spheres
    Specular* shiny1 = new Specular(Vector3(0.8f, 0.8f, 0.8f), Vector3(1.0f), Vector3(0.0f), 250.0f, 0.5f, 0.0f);
    Specular* shiny2 = new Specular(Vector3(1.0f, 0.0f, 0.0f), Vector3(1.0f), Vector3(0.0f), 20.0f, 0.1f, 0.0f);
    Specular* shiny3 = new Specular(Vector3(1.0f, 1.0f, 0.0f), Vector3(1.0f, 1.0f, 0.0f),Vector3(0.0f), 1000.0f, 0.1f, 0.0f);
    const int maxI = 2;
    const float a = 0.25f;
    Sphere * sphere = new Sphere;
    sphere->setCenter(Vector3(-0.3,0,0));
    sphere->setRadius(.25);
    sphere->setMaterial(shiny1);
    g_scene->addObject(sphere);
    sphere = new Sphere;
    sphere->setCenter(Vector3(0.3,0,0));
    sphere->setRadius(.25);
    sphere->setMaterial(shiny2);
    g_scene->addObject(sphere);
    // for (int i = 1; i < maxI; ++i)
    // {
    //     float t = i/float(maxI);
    //     float theta = 4*PI*t;
    //     float r = a*theta;
    //     float x = r*cos(theta);
    //     float y = r*sin(theta);
    //     float z = 2*(2*PI*a - r);
    //     Sphere * sphere = new Sphere;
    //     sphere->setCenter(Vector3(x,y,z));
    //     sphere->setRadius(r/10);
    //     sphere->setMaterial(mat);
    //     g_scene->addObject(sphere);
    // }
    TriangleMesh * floor = new TriangleMesh;
    floor->createSingleTriangle();
    floor->setV1(Vector3(  0, -0.5,  10));
    floor->setV2(Vector3( 10, -0.5, -10));
    floor->setV3(Vector3(-10, -0.5, -10));
    floor->setN1(Vector3(0, 1, 0));
    floor->setN2(Vector3(0, 1, 0));
    floor->setN3(Vector3(0, 1, 0));

    Triangle* t = new Triangle;
    t->setIndex(0);
    t->setMesh(floor);
    t->setMaterial(shiny3);
    g_scene->addObject(t);
    // let objects do pre-calculations if needed
    g_scene->preCalc();
}
