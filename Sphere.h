#ifndef SPHERE_H_
#define SPHERE_H_

#include "Hitable.h"

class Sphere: public Hitable
{
    public:
        Sphere() {}
        Sphere(Vec3 o, float r, Material *m) : origin(o), radius(r), mat_ptr(m) {};
        virtual bool hit(const Ray& r, float t_min, float t_max, hit_record& rec) const;
        
        Vec3 origin;
        float radius;
        Material *mat_ptr;
};

bool Sphere::hit(const Ray& r, float t_min, float t_max, hit_record& rec) const
{
    Vec3 oc = r.origin() - origin;
    float a = dot(r.direction(), r.direction());
    float b = dot(oc, r.direction());
    float c = dot(oc, oc) - radius*radius;
    float disc = b*b - a*c;
    if (disc > 0)
    {
        float temp = (-b - sqrt(b*b-a*c))/a;
        if (temp < t_max && temp > t_min)
        {
            rec.t = temp;
            rec.p = r.point_at_t(rec.t);
            rec.normal = (rec.p - origin) / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }
        temp = (-b + sqrt(b*b-a*c))/a;
        if (temp < t_max && temp > t_min)
        {
            rec.t = temp;
            rec.p = r.point_at_t(rec.t);
            rec.normal - (rec.p - origin) / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }
    }
    return false;
}

#endif
