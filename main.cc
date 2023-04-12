#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "rtweekend.h"

#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "triangle.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <stlfuns.h>

using namespace raytrace;

double hit_sphere(const point3& center, double radius, const ray& r) {
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius*radius;
    auto discriminant = half_b*half_b - a*c;
    if (discriminant < 0) {
        return -1.0;
    } else {
        return (-half_b - sqrt(discriminant) ) / a;
    }
}

color ray_color(const ray& r, const hittable& world) {
   hit_record rec;
   if (world.hit(r, interval(0, infinity), rec)) {
      return 0.5 * (rec.normal + color(1,1,1));
   }
   vec3 unit_direction = unit_vector(r.direction());
   auto a = 0.5*(unit_direction.y() + 1.0);
   return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
}

std::vector<triangle> read_stl() {
   const std::string stl_filename = "../stl-parse/test-files/femur_binary.stl";

   auto S = Stl::readStlFileBinary(stl_filename);

   std::vector<triangle> stl_tris(S.n_triangles);

   for (uint32_t i = 0; i < S.n_triangles; ++i) {
      auto v0 = vec3(S.tris[i].vertices[0].P[0], S.tris[i].vertices[0].P[1], S.tris[i].vertices[0].P[2]);
      auto v1 = vec3(S.tris[i].vertices[1].P[0], S.tris[i].vertices[1].P[1], S.tris[i].vertices[1].P[2]);
      auto v2 = vec3(S.tris[i].vertices[2].P[0], S.tris[i].vertices[2].P[1], S.tris[i].vertices[2].P[2]);

      stl_tris[i] = triangle(v0,v1,v2);
   }

   std::cout << "# of triangles: " << stl_tris.size() << '\n';
   return stl_tris;
}

int main(){

   // Image
   constexpr auto aspect_ratio = 16.0/9.0;
   constexpr int image_width = 400;
   constexpr int image_height = static_cast<int>(image_width / aspect_ratio);
   constexpr int n_channels = 3;

   constexpr int samples_per_pixel = 25;

   char image[image_width * image_height * n_channels];
   std::ofstream outfile;

   // World
   hittable_list world;
   world.add(std::make_shared<sphere>(point3(0,0,-1), 0.5));
   world.add(std::make_shared<sphere>(point3(0,-100.5,-1), 100));

   auto tri_vec = read_stl();

   // Camera
   camera cam;

   outfile.open("image.ppm");
   
   outfile << "P3\n" << image_width << ' ' << image_height << "\n255\n";

   for(int j=0;j<image_height;++j){
      std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
      auto t = (j + random_double()) / (image_height - 1);

      for(int i=0;i<image_width;++i){
         color pixel_color(0,0,0);

         for (int sample = 0; sample < samples_per_pixel; ++sample) {
            auto s = (i + random_double()) / (image_width - 1);
            ray r = cam.get_ray(s,t);
            pixel_color += ray_color(r,world);
         }

         write_color(outfile, pixel_color, samples_per_pixel);

         image[(j*image_width*n_channels) + (n_channels*i) + 0] = static_cast<char>(255.999 * (pixel_color.x() / samples_per_pixel));
         image[(j*image_width*n_channels) + (n_channels*i) + 1] = static_cast<char>(255.999 * (pixel_color.y() / samples_per_pixel));
         image[(j*image_width*n_channels) + (n_channels*i) + 2] = static_cast<char>(255.999 * (pixel_color.z() / samples_per_pixel));
      }
   }

   auto err = stbi_write_png("image.png",image_width,image_height,n_channels,image,(image_width * n_channels));

   outfile.close();
   std::clog << "\rDone                  \n";

   return 0;
}