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
