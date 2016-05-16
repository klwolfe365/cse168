#include "Material.h"

Material::Material()
{
    m_n = 1.0f;
    m_texture = NULL;
}

Material::~Material()
{
}

Vector3
Material::shade(const Ray&, const HitInfo&, const Scene&) const
{
    return Vector3(1.0f, 1.0f, 1.0f);
}


Vector3 Material::reflect(const Vector3 & normal, const Vector3 & incident) const{
    return Vector3(0.0f);
}

Vector3 Material::refract(const Vector3 & normal, const Vector3 & incident,
                double n1, double n2) const{
    return Vector3(0.0f);
}

Vector3 Material::randomReflect(const Vector3& ray, const Vector3& hit, const bool& isFront) {
    double phi = 2.0 * M_PI*(double)rand() / (double)RAND_MAX;
    double theta = acos(2.0*(double)rand() / RAND_MAX - 1.0);
    m_emittance = 1.0/(4.0*M_PI);
    return Vector3(sin(theta)*cos(phi), sin(theta)*sin(phi), cos(theta));
}
