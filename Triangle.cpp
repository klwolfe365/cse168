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
    TriangleMesh::TupleI3 vti3 = m_mesh->vIndices()[m_index];
    const Vector3 & v0 = m_mesh->vertices()[vti3.x]; //vertex a of triangle
    const Vector3 & v1 = m_mesh->vertices()[vti3.y]; //vertex b of triangle
    const Vector3 & v2 = m_mesh->vertices()[vti3.z]; //vertex c of triangle

    /* Vectors to use with Cramer's rule */
    Vector3 a = Vector3(v0.x - v1.x,
                        v0.y - v1.y,
                        v0.z - v1.z);
    Vector3 b = Vector3(v0.x - v2.x,
                        v0.y - v2.y,
                        v0.z - v2.z);
    Vector3 c = Vector3(r.d.x,
                        r.d.y,
                        r.d.z);
    Vector3 d = Vector3(v0.x - r.o.x,
                        v0.y - r.o.y,
                        v0.z - r.o.z);
    /* Compute beta */
    Vector3 crossProd = cross(b,c);
    float det = dot(a, crossProd);
    float detx = dot(d, crossProd);
    float beta = detx / det;
    if (beta < 0 || beta > 1.0)
        return false;

    /* Compute gamma */
    crossProd = cross(a,d);
    float dety = dot(crossProd, c);
    float gamma = dety / det;
    if (gamma < 0 || gamma > (1.0-beta))
        return false;

    /* Compute intersection t */
    float detz = - dot(crossProd, b);
    float t = detz / det;
    if(t < tMin || t > tMax)
        return false;

    /* Store intersection information */
    result.t = t;
    result.P = r.o + (t * r.d);

    /* Barycentric Interpolation of normals */
    TriangleMesh::TupleI3 nti3 = m_mesh->nIndices()[m_index];
    const Vector3 & n0 = m_mesh->normals()[nti3.x];
    const Vector3 & n1 = m_mesh->normals()[nti3.y];
    const Vector3 & n2 = m_mesh->normals()[nti3.z];
    result.N = (((1 - beta - gamma) * n0) + (beta * n1) + (gamma * n2)).normalize();

    result.material = this->m_material;
    return true;
}
