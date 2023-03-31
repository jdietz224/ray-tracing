#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "color.h"
#include "ray.h"
#include "vec3.h"

#include <iostream>
#include <fstream>

bool hit_sphere(const raytrace::point3& center, double radius, const raytrace::ray& r) {
    raytrace::vec3 oc = r.origin() - center;
    auto a = dot(r.direction(), r.direction());
    auto b = 2.0 * dot(oc, r.direction());
    auto c = dot(oc, oc) - radius*radius;
    auto discriminant = b*b - 4*a*c;
    return (discriminant > 0);
}

raytrace::color ray_color(const raytrace::ray& r) {
    if (hit_sphere(raytrace::point3(0,0,-1), 0.5, r))
    {
        return raytrace::color(1, 0, 0);
    }
    raytrace::vec3 unit_direction = raytrace::unit_vector(r.direction());
    auto a = 0.5*(unit_direction.y() + 1.0);
    return (1.0-a)*raytrace::color(1.0, 1.0, 1.0) + a*raytrace::color(0.5, 0.7, 1.0);
}

int main(){

   // Image
   constexpr auto aspect_ratio = 16.0/9.0;
   constexpr int image_width = 400;
   constexpr int image_height = static_cast<int>(image_width / aspect_ratio);
   constexpr int n_channels = 3;

   char image[image_width * image_height * n_channels];
   std::ofstream outfile;

   // Camera
    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    auto origin = raytrace::point3(0, 0, 0);
    auto horizontal = raytrace::vec3(viewport_width, 0, 0);
    auto vertical = raytrace::vec3(0, viewport_height, 0);
    auto lower_left_corner = origin - horizontal/2 - vertical/2 - raytrace::vec3(0, 0, focal_length);

   outfile.open("image.ppm");
   
   outfile << "P3\n" << image_width << ' ' << image_height << "\n255\n";

   for(int j=0;j<image_height;++j){
      std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
      for(int i=0;i<image_width;++i){
         auto s = double(i) / (image_width-1);
         auto t = double(j) / (image_height-1);
         raytrace::ray r(origin, lower_left_corner + s*horizontal + (1-t)*vertical - origin);
         raytrace::color pixel_color = ray_color(r);
         raytrace::write_color(outfile, pixel_color);

         image[(j*image_width*n_channels) + (n_channels*i) + 0] = static_cast<char>(255.999 * pixel_color.x());
         image[(j*image_width*n_channels) + (n_channels*i) + 1] = static_cast<char>(255.999 * pixel_color.y());
         image[(j*image_width*n_channels) + (n_channels*i) + 2] = static_cast<char>(255.999 * pixel_color.z());
      }
   }

   auto err = stbi_write_png("image.png",image_width,image_height,n_channels,image,(image_width * n_channels));

   std::clog << "\rDone                  \n";

   return 0;
}