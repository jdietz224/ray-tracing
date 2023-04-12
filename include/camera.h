#pragma once

#include "rtweekend.h"

namespace raytrace{
  class camera {
    public:
      camera() {
          auto aspect_ratio = 16.0 / 9.0;
          auto viewport_height = 2.0;
          auto viewport_width = aspect_ratio * viewport_height;
          auto focal_length = 1.0;

          origin = point3(0, 0, 0);
          horizontal = vec3(viewport_width, 0.0, 0.0);
          vertical = vec3(0.0, viewport_height, 0.0);
          lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0, 0, focal_length);
      }

      ray get_ray(double s, double t) const {
          // Return the ray from the projection point to the indicated pixel. Coordinates s,t are
          // the normalized image-based coordinates of the pixel. Image left is s=0, image right
          // is s=1, image top is t=0, image bottom is t=1.

          return ray(origin, lower_left_corner + s*horizontal + (1-t)*vertical - origin);
      }

    private:
      point3 origin;
      point3 lower_left_corner;
      vec3 horizontal;
      vec3 vertical;
  };
}