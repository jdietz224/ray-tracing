#pragma once

#include <array>
#include "hittable.h"
#include "vec3.h"

namespace raytrace
{
    class triangle : public hittable {
    public:
        triangle() {}
        triangle(std::array<point3,3> points) : vertices(points) {};
        triangle(point3 v1, point3 v2, point3 v3) : vertices({v1, v2, v3}) {};

        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {

            constexpr double SMALL_NUM = 1e-8;

            // Construct triangle edge vectors and plane normal
            auto u = vertices[1] - vertices[0];
            auto v = vertices[2] - vertices[0];
            auto n = cross(u,v);
            auto w0 = r.orig - vertices[0];

            auto a = -dot(n,w0);
            auto b = dot(n,r.dir);

            // Ray is parallel to the plane
            if (fabs(b) < SMALL_NUM) {
                // Ray lies in the triangle plane
                if (a == 0) return true;
                // Ray lies outside the triangle plane
                return false;
            }

            auto d = a / b;

            // If the intersect point is outside the viable interval, no hit
            if ((d < ray_t.min) || (d > ray_t.max)) return false;

            // Get the intersect point of ray with triangle plane
            auto I = r.orig + d*r.dir;

            // Now test if intersect point is inside the triangle
            auto uu = dot(u,u);
            auto uv = dot(u,v);
            auto vv = dot(v,v);
            auto w = I - vertices[0];
            auto wu = dot(w,u);
            auto wv = dot(w,v);

            auto D = (uv * uv) - (uu * vv);

            // get and test parametric coordinates
            auto s = ((uv * wv) - (vv * wu)) / D;
            if ((s < 0.0) || (s > 1.0)) return false;

            auto t = ((uv * wu) - (uu * wv)) / D;
            if ((t < 0.0) || (s + t) > 1.0) return false;

            return true;
        }
    public:
        std::array<point3,3> vertices;
    };
}