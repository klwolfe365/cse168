#include "Triangle.h"
#include "TriangleMesh.h"
#include "Ray.h"


Triangle::Triangle(TriangleMesh * m, unsigned int i) :
    m_mesh(m), m_index(i)
{

}


Triangle::~Triangle()
{

}


void
Triangle::renderGL()
{
    TriangleMesh::TupleI3 ti3 = m_mesh->vIndices()[m_index];
    const Vector3 & v0 = m_mesh->vertices()[ti3.x]; //vertex a of triangle
    const Vector3 & v1 = m_mesh->vertices()[ti3.y]; //vertex b of triangle
    const Vector3 & v2 = m_mesh->vertices()[ti3.z]; //vertex c of triangle

    glBegin(GL_TRIANGLES);
        glVertex3f(v0.x, v0.y, v0.z);
        glVertex3f(v1.x, v1.y, v1.z);
        glVertex3f(v2.x, v2.y, v2.z);
    glEnd();
}



bool
Triangle::intersect(HitInfo& result, const Ray& r,float tMin, float tMax)
{
    TriangleMesh::TupleI3 ti3 = m_mesh->vIndices()[m_index];
    const Vector3 & v0 = m_mesh->vertices()[ti3.x]; //vertex a of triangle
    const Vector3 & v1 = m_mesh->vertices()[ti3.y]; //vertex b of triangle
    const Vector3 & v2 = m_mesh->vertices()[ti3.z]; //vertex c of triangle

    float a = v0.x - v1.x;
    float d = v0.x - v2.x;
    float g = r.d.x;

    float b = v0.y - v1.y;
    float e = v0.y - v2.y;
    float h = r.d.y;

    float c = v0.z - v1.z;
    float f = v0.z - v2.z;
    float i = r.d.z;

    float j = v0.x - r.o.x;
    float k = v0.y - r.o.y;
    float l = v0.z - r.o.z;

    float m = a*(e*i - h*f) + b*(g*f - d*i) + c*(d*h - e*g);

    float t = (f*(a*k - j*b) + e*(j*c - a*l) + d*(b*l - k*c))/m;
    if(t < tMin || t > tMax){
        return false;
    }
    result.t = t;

    float gamma = (i*(a*k - j*b) + h*(j*c - a*l) + g*(b*l - k*c))/m;
    if(gamma < 0 || gamma > 1){
        return false;
    }

    float beta = (j*(e*i - h*f) + k*(g*f - d*i) + l*(d*h - e*g))/m;
    if(beta < 0 || beta > 1){
        return false;
    }

    result.P = r.o + t*r.d;

    return true;
}
