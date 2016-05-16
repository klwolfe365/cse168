#include "Lambert.h"
#include "Ray.h"
#include "Scene.h"
#include <algorithm>

Lambert::Lambert(const Vector3 & kd, const Vector3 & ka) :
    m_kd(kd), m_ka(ka)
{

}

Lambert::~Lambert()
{
}

Vector3
Lambert::shade(const Ray& ray, const HitInfo& hit, const Scene& scene) const
{
    // printf("Shading...\n");
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
        Vector3 result = pLight->color();
        result *= color;

        L += std::max(0.0f, nDotL/falloff * pLight->wattage() / PI) * result;
    }

    // add the ambient component
    L += m_ka;

    return L;
}

Vector3 Lambert::reflect(const Vector3 & normal, const Vector3 & incident) const{
    return m_kd;
}

Vector3 Lambert::refract(const Vector3 & normal, const Vector3 & incident,
                double n1, double n2) const{
    return Vector3(0.0f);
}

Vector3 Lambert::randomReflect(Vector3 in, Vector3 hitNorm) const {
    float u;
    do {
        u = ((float)rand() / RAND_MAX);
    } while (u == 1.0);

    float v = 2.0 * M_PI*((float)rand() / RAND_MAX);

    Vector3 dir = Vector3(cos(v)*sqrt(u), sin(v)*sqrt(u), sqrt(1 - u));
    Vector3 n = hitNorm.normalized();
    //Generate orthonormal basis
    Vector3 a = dot(Vector3(1,0,0), n);
    Vector3 b = dot(Vector3(0,1,0), n);
    Vector3 y;
    if(fabs(a) < fabs(b)){
        y = Vector3(1,0,0)-(dot(Vector(1,0,0), n)*n);
    } else {
        y = Vector3(0,1,0)-(dot(Vector(0,1,0), n)*n);
    }
    Vector3 x = cross(y, n).normalize();
    Vector3 result = dir.x*x + dir.y*y + dir.z*n;
    return result;
}
