#pragma once

#include "hittable.h"
#include "vec3.h"

namespace raytrace
{
    class sphere : public hittable {
    public:
        sphere() {}
        sphere(point3 ctr, double r) : center(ctr), radius(r) {};

        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            vec3 oc = r.origin() - center;
            auto a = r.direction().length_squared();
            auto half_b = dot(oc, r.direction());
            auto c = oc.length_squared() - radius*radius;

            auto discriminant = half_b*half_b - a*c;
            if (discriminant < 0) return false;
            auto sqrtd = sqrt(discriminant);

            // Find the nearest root that lies in the acceptable range.
            auto root = (-half_b - sqrtd) / a;
            if (!ray_t.contains(root)) {
                root = (-half_b + sqrtd) / a;
                if (!ray_t.contains(root))
                    return false;
            }

            rec.t = root;
            rec.p = r.at(rec.t);
            vec3 outward_normal = (rec.p - center) / radius;
            rec.set_face_normal(r, outward_normal);

            return true;
        }

    public:
        point3 center;
        double radius;
    };
}