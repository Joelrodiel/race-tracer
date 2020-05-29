#ifndef MATERIAL_H_
#define MATERIAL_H_

struct hit_record;

#include "Ray.h"
#include "Hitable.h"

Vec3 random_in_unit_sphere() {
    Vec3 p;
    do {
        p = 2.0*Vec3((rand() / (RAND_MAX + 1.0)),(rand() / (RAND_MAX + 1.0)),(rand() / (RAND_MAX + 1.0))) - Vec3(1,1,1);
    } while (p.squared_length() >= 1.0);
    return p;
}

class Material
{
    public:
        virtual bool Scatter(const Ray& r_in, const hit_record& rec, Vec3& attenuation, Ray& scattered) const = 0;
};

class Lambertian: public Material
{
    public:
        Lambertian(const Vec3& a) : albedo(a) {}

        virtual bool Scatter(const Ray& r_in, const hit_record& rec, Vec3& attenuation, Ray& scattered) const
        {
            Vec3 target = rec.p + rec.normal + random_in_unit_sphere();
            scattered = Ray(rec.p, target-rec.p);
            attenuation = albedo;
            return true;
        }

        Vec3 albedo;
};

#endif
