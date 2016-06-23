//
//  main.cpp
//  cse168
//
//  Provided in CSE168 starter code
//  Modified by Karen Wolfe
//
//
#include <math.h>
#include "Miro.h"
#include "Scene.h"
#include "Camera.h"
#include "Image.h"
#include "Console.h"

#include "PointLight.h"
#include "Sphere.h"
#include "TriangleMesh.h"
#include "Triangle.h"
#include "Lambert.h"
#include "CellShading.h"
#include "EdgeShading.h"
#include "CoolWarmShader.h"
#include "PencilShader.h"

#include "Specular.h"
#include "MiroWindow.h"
#include "assignment2.h"

void
makeSpiralScene()
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
    light->setWattage(1000);
    g_scene->addLight(light);

    // create a spiral of spheres
    Material* mat = new Lambert(Vector3(1.0f, 0.0f, 0.0f));
    // Material* mat = new Specular(Vector3(1.0f, 0.0f, 0.0f), Vector3(0.5f), 0.5f);
    const int maxI = 200;
    const float a = 0.15f;
    for (int i = 1; i < maxI; ++i)
    {
        float t = i/float(maxI);
        float theta = 4*PI*t;
        float r = a*theta;
        float x = r*cos(theta);
        float y = r*sin(theta);
        float z = 2*(2*PI*a - r);
        Sphere * sphere = new Sphere;
        sphere->setCenter(Vector3(x,y,z));
        sphere->setRadius(r/10);
        sphere->setMaterial(mat);
        g_scene->addObject(sphere);
    }

    // let objects do pre-calculations if needed
    g_scene->preCalc();
}

void makeTwoTriangles(){
    g_camera = new Camera;
    g_scene = new Scene;
    g_image = new Image;

    g_image->resize(512, 512);

    // set up the camera
    g_camera->setBGColor(Vector3(0.0f, 0.0f, 0.2f));
    g_camera->setEye(Vector3(0, 3, 6));
    g_camera->setLookAt(Vector3(0, 0, 0));
    g_camera->setUp(Vector3(0, 1, 0));
    g_camera->setFOV(45);

    // create and place a point light source
    PointLight * light = new PointLight;
    light->setPosition(Vector3(10, 10, 10));
    light->setColor(Vector3(1, 1, 1));
    light->setWattage(700);
    g_scene->addLight(light);

    Material* material = new Lambert(Vector3(1.0f));

    TriangleMesh * floor = new TriangleMesh;
    floor->createSingleTriangle();
    floor->setV1(Vector3(-2, 0, 0));
    floor->setV2(Vector3( 0, 0,  0));
    floor->setV3(Vector3( -1, 3, 0));
    floor->setN1(Vector3(0, 1, 0));
    floor->setN2(Vector3(0, 1, 0));
    floor->setN3(Vector3(0, 1, 0));

    Triangle* t = new Triangle;
    t->setIndex(0);
    t->setMesh(floor);
    t->setMaterial(material);
    g_scene->addObject(t);

    TriangleMesh * floor2 = new TriangleMesh;
    floor2->createSingleTriangle();
    floor2->setV1(Vector3(1, 3, -3));
    floor2->setV2(Vector3( 0, 0,  -3));
    floor2->setV3(Vector3( 2, 0, -3));
    floor2->setN1(Vector3(0, 1, -3));
    floor2->setN2(Vector3(0, 1, -3));
    floor2->setN3(Vector3(0, 1, -3));

    Triangle* t2 = new Triangle;
    t2->setIndex(0);
    t2->setMesh(floor2);
    t2->setMaterial(material);
    g_scene->addObject(t2);

    // let objects do pre-calculations if needed
    g_scene->preCalc();
}

void
addMeshTrianglesToScene(TriangleMesh * mesh, Material * material)
{
    printf("Adding mesh");
    // create all the triangles in the bunny mesh and add to the scene
    for (int i = 0; i < mesh->numTris(); ++i)
    {
        Triangle* t = new Triangle;
        t->setIndex(i);
        t->setMesh(mesh);
        t->setMaterial(material);
        g_scene->addObject(t);
    }
}

inline Matrix4x4
translate(float x, float y, float z)
{
    Matrix4x4 m;
    m.setColumn4(Vector4(x, y, z, 1));
    return m;
}


inline Matrix4x4
scale(float x, float y, float z)
{
    Matrix4x4 m;
    m.m11 = x;
    m.m22 = y;
    m.m33 = z;
    return m;
}

// angle is in degrees
inline Matrix4x4
rotate(float angle, float x, float y, float z)
{
    float rad = angle*(PI/180.);

    float x2 = x*x;
    float y2 = y*y;
    float z2 = z*z;
    float c = cos(rad);
    float cinv = 1-c;
    float s = sin(rad);
    float xy = x*y;
    float xz = x*z;
    float yz = y*z;
    float xs = x*s;
    float ys = y*s;
    float zs = z*s;
    float xzcinv = xz*cinv;
    float xycinv = xy*cinv;
    float yzcinv = yz*cinv;

    Matrix4x4 m;
    m.set(x2 + c*(1-x2), xy*cinv+zs, xzcinv - ys, 0,
          xycinv - zs, y2 + c*(1-y2), yzcinv + xs, 0,
          xzcinv + ys, yzcinv - xs, z2 + c*(1-z2), 0,
          0, 0, 0, 1);
    return m;
}

//@author Karen Wolfe
/* Scene to draw forest scene with bunnies, log, cabin, and rock using
 * different textures */
void makeCartoonScene(){
    g_camera = new Camera;
    g_scene = new Scene;
    g_image = new Image;

    g_image->resize(700, 700);

    // set up the camera
    g_camera->setBGColor(Vector3(0.7f, 0.95f, 0.99f));
    g_camera->setEye(Vector3(0, 4, 15));
    g_camera->setLookAt(Vector3(0, 1, 0));
    g_camera->setUp(Vector3(0, 1, 0));
    g_camera->setFOV(45);

    // create and place a point light source
    PointLight * light = new PointLight;
    light->setPosition(Vector3(10, 10, 0));
    light->setColor(Vector3(1, 1, 1));
    light->setWattage(700);
    g_scene->addLight(light);

    Material* cartoon = new EdgeShading(Vector3(0.80f, 0.6f, 0.28f), Vector3(0.0f), 0.2, 0.8, 0.99);
    Material* material = new Lambert(Vector3(1.0f));
    PencilShader* sky = new PencilShader();//EdgeShading(Vector3(0.2f, 0.90f, 0.99f), Vector3(0.0f), 0.3, 0.6, 1.0);
    sky->loadTexture("textures/paper2.ppm");

    PencilShader* sketchShade = new PencilShader();
    std::vector<const char *> sTextures;
    sTextures.push_back("textures/paper2.ppm");
    sTextures.push_back("textures/hatch_0.ppm");
    sTextures.push_back("textures/hatch_1.ppm");
    sTextures.push_back("textures/hatch_2.ppm");
    sTextures.push_back("textures/hatch_3.ppm");
    sTextures.push_back("textures/hatch_4.ppm");
    sTextures.push_back("textures/hatch_5.ppm");
    sketchShade->loadTextures(sTextures);

    Matrix4x4 xform;
    xform.setIdentity();
    xform *= translate(3, -3, 2);
    xform *= scale(1.3, 1.3, 1.3);
    TriangleMesh * bunny1 = new TriangleMesh;
    bunny1->objId = 0;
    bunny1->load("bunny.obj", xform);

    addMeshTrianglesToScene(bunny1, cartoon);

    Material* b2 = new EdgeShading(Vector3(0.67f, 0.67f, 0.67f), Vector3(0.0f), 0.15, 0.6, 1.0);
    xform.setIdentity();
    xform *= rotate(180, 0, 1, 0);
    xform *= translate(2, -3, -3);
    xform *= scale(1.2, 1.2, 1.2);
    TriangleMesh * bunny2 = new TriangleMesh;
    bunny2->objId = 1;
    bunny2->load("bunny.obj", xform);
    addMeshTrianglesToScene(bunny2, b2);

    Material* wood = new EdgeShading(Vector3(0.5f, 0.29f, 0.0f), Vector3(0.0f), 0.3, 0.6, 1.0);
    xform.setIdentity();
    xform *= rotate(20, 1, 0, 0);
    xform *= translate(0, -1, -8);
    xform *= scale(0.9, 0.6, 0.4);
    TriangleMesh * woodLog = new TriangleMesh;
    woodLog->objId = 2;
    woodLog->load("objs/log2_low.obj", xform);
    addMeshTrianglesToScene(woodLog, wood);

    Material* rock = new EdgeShading(Vector3(0.435f, 0.407f, 0.39f), Vector3(0.0f), 0.3, 0.6, 0.85);
    xform.setIdentity();
    xform *= translate(6, -3, -6.5);
    xform *= scale(0.05, 0.05, 0.025);
    TriangleMesh * stone = new TriangleMesh;
    stone->objId = 3;
    stone->load("objs/stone2.obj", xform);
    addMeshTrianglesToScene(stone, sketchShade);

    xform.setIdentity();
    xform += rotate(30, 0, 1, 0);
    xform *= scale(0.05, 0.05, 0.04);
    xform *= translate(-40, -10, -80);
    TriangleMesh * house = new TriangleMesh;
    house->objId = 6;
    house->load("objs/WoodenCabinObj.obj", xform);
    addMeshTrianglesToScene(house, sketchShade);

    // create the floor triangle
    Material* grass = new EdgeShading(Vector3(0.20f, 0.76f, 0.317f), Vector3(0.0f), 0.3, 0.6, 1.0);
    TriangleMesh * floor = new TriangleMesh;
    floor->objId = 4;
    floor->createSingleTriangle();
    floor->setV1(Vector3(-15, -3, -10));
    floor->setV2(Vector3(  0, -3,  15));
    floor->setV3(Vector3( 15, -3, -10));
    floor->setN1(Vector3(0, 1, 0));
    floor->setN2(Vector3(0, 1, 0));
    floor->setN3(Vector3(0, 1, 0));

    Triangle* t = new Triangle;
    t->setIndex(0);
    t->setMesh(floor);
    t->setMaterial(grass);
    g_scene->addObject(t);

    TriangleMesh * bg = new TriangleMesh;
    bg->objId = 5;
    bg->createSingleTriangle();
    bg->setV1(Vector3( 25, -3, -10));
    bg->setV2(Vector3( 0, 20,  -10));
    bg->setV3(Vector3( -25, -3, -10));
    bg->setN1(Vector3(0, 0, 1));
    bg->setN2(Vector3(0, 0, 1));
    bg->setN3(Vector3(0, 0, 1));

    t = new Triangle;
    t->setIndex(0);
    t->setMesh(bg);
    t->setMaterial(sky);
    g_scene->addObject(t);

    // let objects do pre-calculations if needed
    g_scene->preCalc();
}
int
main(int argc, char*argv[])
{
    makeCartoonScene();
    MiroWindow miro(&argc, argv);
    miro.mainLoop();

    return 0; // never executed
}
