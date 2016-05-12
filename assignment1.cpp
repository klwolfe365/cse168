// #include "assignment1.h"
// #include <math.h>
// #include "Miro.h"
// #include "Scene.h"
// #include "Camera.h"
// #include "Image.h"
// #include "Console.h"
//
// #include "PointLight.h"
// #include "TriangleMesh.h"
// #include "Triangle.h"
// #include "Lambert.h"
// #include "Specular.h"
// #include "Sphere.h"
// #include "StoneTexture.h"
//
// void
// makeBunnyScene()
// {
//     g_camera = new Camera;
//     g_scene = new Scene;
//     g_image = new Image;
//
//     g_image->resize(128, 128);
//
//     // set up the camera
//     g_camera->setBGColor(Vector3(0.0f));
//     g_scene->setBGColor(Vector3(0.0f));
//     g_camera->setEye(Vector3(-2, 3, 5));
//     g_camera->setLookAt(Vector3(-.5, 1, 0));
//     g_camera->setUp(Vector3(0, 1, 0));
//     g_camera->setFOV(45);
//
//     // create and place a point light source
//     PointLight * light = new PointLight;
//     light->setPosition(Vector3(-3, 15, 3));
//     light->setColor(Vector3(1, 1, 1));
//     light->setWattage(500);
//     g_scene->addLight(light);
//
//     Material* matte = new Lambert(Vector3(0.7f, 0.7f, 0.7f));
//     // Material* matte2 = new Lambert(Vector3(0.7f, 0.5f, 0.7f));
//     Specular* shiny = new Specular(Vector3(0.7f, 0.5f, 0.7f), Vector3(1.0f), 10.0f, 0.0f, 1.88f, 0.9f);
//     StoneTexture* stone = new StoneTexture(5.0, 2);
//
//     // matte->setTexture(stone);
//
//     TriangleMesh * bunny = new TriangleMesh;
//     bunny->load("bunny.obj");
//
//     // create all the triangles in the bunny mesh and add to the scene
//     for (int i = 0; i < bunny->numTris(); ++i)
//     {
//         Triangle* t = new Triangle;
//         t->setIndex(i);
//         t->setMesh(bunny);
//         t->setMaterial(shiny);
//         g_scene->addObject(t);
//     }
//
//     // create the floor triangle
//     TriangleMesh * floor = new TriangleMesh;
//     floor->createSingleTriangle();
//     floor->setV1(Vector3(  0, 0,  10));
//     floor->setV2(Vector3( 10, 0, -10));
//     floor->setV3(Vector3(-10, 0, -10));
//     floor->setN1(Vector3(0, 1, 0));
//     floor->setN2(Vector3(0, 1, 0));
//     floor->setN3(Vector3(0, 1, 0));
//
//     Triangle* t = new Triangle;
//     t->setIndex(0);
//     t->setMesh(floor);
//     t->setMaterial(matte);
//     g_scene->addObject(t);
//
//     // let objects do pre-calculations if needed
//     g_scene->preCalc();
// }
//
// void
// makeSphereScene()
// {
//     g_camera = new Camera;
//     g_scene = new Scene;
//     g_image = new Image;
//
//     g_image->resize(512, 512);
//
//     // set up the camera
//     g_camera->setBGColor(Vector3(1.0f, 1.0f, 1.0f));
//     g_scene->setBGColor(Vector3(1.0f, 1.0f, 1.0f));
//     g_camera->setEye(Vector3(-2, 1, -1));
//     g_camera->setLookAt(Vector3(0, 0, 0));
//     g_camera->setUp(Vector3(0, 1, 0));
//     g_camera->setFOV(45);
//
//     // create and place a point light source
//     PointLight * light = new PointLight;
//     light->setPosition(Vector3(-3, 15, 3));
//     light->setColor(Vector3(1, 1, 1));
//     light->setWattage(700);
//     g_scene->addLight(light);
//
//     // create a spiral of spheres
//     // Material* mat = new Lambert(Vector3(1.0f, 0.0f, 0.0f));
//     Material* matte = new Lambert(Vector3(1.0f, 1.0f, 1.0f));
//     Material* matte2 = new Lambert(Vector3(0.5f, 1.0f, 0.9f));
//     Material* shiny = new Specular(Vector3(0.5f, 1.0f, 0.9f), Vector3(1.0f), 100.0f, 0.1f, 2.5f, 4.0f);
//     TriangleMesh * sphere = new TriangleMesh;
//     sphere->load("objs/sphere.obj");
//
//     // create all the triangles in the bunny mesh and add to the scene
//     for (int i = 0; i < sphere->numTris(); ++i)
//     {
//         Triangle* t = new Triangle;
//         t->setIndex(i);
//         t->setMesh(sphere);
//         t->setMaterial(shiny);
//         g_scene->addObject(t);
//     }
//
//     // create the floor triangle
//     TriangleMesh * floor = new TriangleMesh;
//     floor->createSingleTriangle();
//     floor->setV1(Vector3(  0, -1,  20));
//     floor->setV2(Vector3( 10, -1, -20));
//     floor->setV3(Vector3(-10, -1, -20));
//     floor->setN1(Vector3(0, 1, 0));
//     floor->setN2(Vector3(0, 1, 0));
//     floor->setN3(Vector3(0, 1, 0));
//
//     Triangle* t = new Triangle;
//     t->setIndex(0);
//     t->setMesh(floor);
//     t->setMaterial(matte);
//     g_scene->addObject(t);
//
//     // let objects do pre-calculations if needed
//     g_scene->preCalc();
// }
//
// void
// makeTeapotScene()
// {
//     g_camera = new Camera;
//     g_scene = new Scene;
//     g_image = new Image;
//
//     g_image->resize(128*3, 128*3.1);
//
//     // set up the camera
//     g_camera->setBGColor(Vector3(0.6f, 1.0f, 1.0f));
//     g_scene->setBGColor(Vector3(0.6f, 1.0f, 1.0f));
//     g_camera->setEye(Vector3(-2, 3, 5));
//     g_camera->setLookAt(Vector3(-.5, 1, 0));
//     g_camera->setUp(Vector3(0, 1, 0));
//     g_camera->setFOV(45);
//
//     // create and place a point light source
//     PointLight * light = new PointLight;
//     light->setPosition(Vector3(-1, 10, 10));
//     light->setColor(Vector3(1.0, 1.0, 1.0));
//     light->setWattage(500);
//     g_scene->addLight(light);
//     // light = new PointLight;
//     // light->setPosition(Vector3(10,0,-3));
//     // light->setColor(Vector3(1.0, 1.0, 1.0));
//     // light->setWattage(5);
//     // g_scene->addLight(light);
//     Material*  matte = Material* matte2 = new Lambert(Vector3(0.8f, 0.7f, 0.9f));//new Specular(Vector3(0.9f, 0.10f, 0.10f),Vector3(1.0f), 10.0f, 0.3f, 2.1, 0.5f);
//     // Specular* matte = new Specular(Vector3(0.7f), Vector3(1.0f), 10.0f, 1.0f, 2.417, 1.0f);
//
//     StoneTexture* stone = new StoneTexture(2.0, 2);
//
//     // Material* matte = new Specular(Vector3(0.0f, 1.0f, 1.0f), Vector3(0.6f, 0.7f, 0.7f), 10.0f, 0.0f, 1.33f, 1.0f);
//     Material* shiny = new Specular(Vector3(0.7f, 0.2f, 0.2f), Vector3(0.7f, 0.2f, 0.2f), 100.0f, 0.5f, 2.147f, 0.4f);
//     matte->setTexture(stone);
//     // shiny->setTexture(stone);
//
//     TriangleMesh * teapot = new TriangleMesh;
//     teapot->load("objs/teapot.obj");
//
//     // create all the triangles in the bunny mesh and add to the scene
//     for (int i = 0; i < teapot->numTris(); ++i)
//     {
//         Triangle* t = new Triangle;
//         t->setIndex(i);
//         t->setMesh(teapot);
//         t->setMaterial(shiny);
//         g_scene->addObject(t);
//     }
//
//     // create the floor triangle
//     TriangleMesh * floor = new TriangleMesh;
//     floor->createSingleTriangle();
//     floor->setV1(Vector3(  0, 01.1,  3));
//     floor->setV2(Vector3( 10, 01.1, -3));
//     floor->setV3(Vector3(-10, 01.1, -3));
//     floor->setN1(Vector3(0, 1, 0));
//     floor->setN2(Vector3(0, 1, 0));
//     floor->setN3(Vector3(0, 1, 0));
//
//     Triangle* t = new Triangle;
//     t->setIndex(0);
//     t->setMesh(floor);
//     t->setMaterial(matte);
//     g_scene->addObject(t);
//
//     // let objects do pre-calculations if needed
//     g_scene->preCalc();
// }
//
// void makeTwoSphereScene(){
//     g_camera = new Camera;
//     g_scene = new Scene;
//     g_image = new Image;
//
//     g_image->resize(512, 512);
//
//     // set up the camera
//     g_camera->setBGColor(Vector3(1.0f, 1.0f, 1.0f));
//     g_scene->setBGColor(Vector3(.0f, 1.0f, 1.0f));
//     g_camera->setEye(Vector3(0, 2, -2));
//     g_camera->setLookAt(Vector3(0, 0, 0));
//     g_camera->setUp(Vector3(0, 1, 0));
//     g_camera->setFOV(45);
//
//     // create and place a point light source
//     PointLight * light = new PointLight;
//     light->setPosition(Vector3(-3, 15, 5));
//     light->setColor(Vector3(1, 1, 1));
//     light->setWattage(800);
//     g_scene->addLight(light);
//
//     // create a spiral of spheres
//     Specular* shiny1 = new Specular(Vector3(0.8f, 0.8f, 0.8f), Vector3(1.0f), 250.0f, 2.0f, 2.147f, 0.9f);
//     Specular* shiny2 = new Specular(Vector3(1.0f, 0.0f, 0.0f), Vector3(1.0f), 20.0f, 2.0f, 1.0f, 0.9f);
//     Specular* shiny3 = new Specular(Vector3(1.0f, 1.0f, 0.0f), Vector3(1.0f), 10.0f, 0.9f, 1.0f, 0.9f);
//     const int maxI = 2;
//     const float a = 0.25f;
//     Sphere * sphere = new Sphere;
//     sphere->setCenter(Vector3(-0.2,0.3,0));
//     sphere->setRadius(.25);
//     sphere->setMaterial(shiny1);
//     g_scene->addObject(sphere);
//     sphere = new Sphere;
//     sphere->setCenter(Vector3(0.1,0,0));
//     sphere->setRadius(.25);
//     sphere->setMaterial(shiny2);
//     g_scene->addObject(sphere);
//     // for (int i = 1; i < maxI; ++i)
//     // {
//     //     float t = i/float(maxI);
//     //     float theta = 4*PI*t;
//     //     float r = a*theta;
//     //     float x = r*cos(theta);
//     //     float y = r*sin(theta);
//     //     float z = 2*(2*PI*a - r);
//     //     Sphere * sphere = new Sphere;
//     //     sphere->setCenter(Vector3(x,y,z));
//     //     sphere->setRadius(r/10);
//     //     sphere->setMaterial(mat);
//     //     g_scene->addObject(sphere);
//     // }
//     TriangleMesh * floor = new TriangleMesh;
//     floor->createSingleTriangle();
//     floor->setV1(Vector3(  0, -0.5,  10));
//     floor->setV2(Vector3( 10, -0.5, -10));
//     floor->setV3(Vector3(-10, -0.5, -10));
//     floor->setN1(Vector3(0, 1, 0));
//     floor->setN2(Vector3(0, 1, 0));
//     floor->setN3(Vector3(0, 1, 0));
//
//     Triangle* t = new Triangle;
//     t->setIndex(0);
//     t->setMesh(floor);
//     t->setMaterial(shiny3);
//     g_scene->addObject(t);
//     // let objects do pre-calculations if needed
//     g_scene->preCalc();
// }
//
// void makeSphereScene2(){
//     g_camera = new Camera;
//     g_scene = new Scene;
//     g_image = new Image;
//
//     g_image->resize(512, 512);
//
//     // set up the camera
//     g_camera->setBGColor(Vector3(0.7f, 0.6f, 0.9f));
//     g_scene->setBGColor(Vector3(0.7f, 0.6f, 0.9f));
//     g_camera->setEye(Vector3(-1, 2, -1));
//     g_camera->setLookAt(Vector3(0, 0, 0));
//     g_camera->setUp(Vector3(0, 1, 0));
//     g_camera->setFOV(45);
//
//     // create and place a point light source
//     PointLight * light = new PointLight;
//     light->setPosition(Vector3(-3, 15, 5));
//     light->setColor(Vector3(1, 1, 1));
//     light->setWattage(500);
//     g_scene->addLight(light);
//
//     // create a spiral of spheres
//     Material* matte = new Lambert(Vector3(0.5f, 1.0f, 0.9f));
//
//     Material* matte2 = new Lambert(Vector3(0.8f, 0.7f, 0.9f));
//     Specular* shiny1 = new Specular(Vector3(0.5f, 1.0f, 0.9f), Vector3(0.5f, 1.0f, 0.9f), 120.0f, 0.6f, 2.147f, 0.8f);
//     Specular* shiny2 = new Specular(Vector3(0.8f, 0.7f, 0.9f), Vector3(0.8f, 0.7f, 0.9f), 20.0f, 0.0f, 1.5f, 0.2f);
//     Specular* shiny3 = new Specular(Vector3(1.0f, 1.0f, 0.0f), Vector3(1.0f), 10.0f, 0.9f, 1.0f, 0.9f);
//     const int maxI = 2;
//     const float a = 0.25f;
//     Sphere * sphere = new Sphere;
//     sphere->setCenter(Vector3(-0.2,0.3,0));
//     sphere->setRadius(.25);
//     sphere->setMaterial(shiny1);
//     g_scene->addObject(sphere);
//     // for (int i = 1; i < maxI; ++i)
//     // {
//     //     float t = i/float(maxI);
//     //     float theta = 4*PI*t;
//     //     float r = a*theta;
//     //     float x = r*cos(theta);
//     //     float y = r*sin(theta);
//     //     float z = 2*(2*PI*a - r);
//     //     Sphere * sphere = new Sphere;
//     //     sphere->setCenter(Vector3(x,y,z));
//     //     sphere->setRadius(r/10);
//     //     sphere->setMaterial(mat);
//     //     g_scene->addObject(sphere);
//     // }
//     TriangleMesh * floor = new TriangleMesh;
//     floor->createSingleTriangle();
//     floor->setV1(Vector3(  0, 0.1,  10));
//     floor->setV2(Vector3( 10, 0.1, -10));
//     floor->setV3(Vector3(-10, 0.1, -10));
//     floor->setN1(Vector3(0, 1, 0));
//     floor->setN2(Vector3(0, 1, 0));
//     floor->setN3(Vector3(0, 1, 0));
//     StoneTexture * stone = new StoneTexture(2.5f, 2);
//     shiny2->setTexture(stone);
//     Triangle* t = new Triangle;
//     t->setIndex(0);
//     t->setMesh(floor);
//     t->setMaterial(shiny2);
//     g_scene->addObject(t);
//     // let objects do pre-calculations if needed
//     g_scene->preCalc();
// }
