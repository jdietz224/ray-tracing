#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "color.h"
#include "vec3.h"

#include <iostream>
#include <fstream>

int main(){
   constexpr int image_width = 1920;
   constexpr int image_height = 1080;
   constexpr int n_channels = 3;

   char image[image_width * image_height * n_channels];
   std::ofstream outfile;

   outfile.open("image.ppm");
   
   outfile << "P3\n" << image_width << ' ' << image_height << "\n255\n";

   for(int j=0;j<image_height;++j){
      std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
      for(int i=0;i<image_width;++i){
         compgeom::color pixel_color(double(i)/(image_width-1), double(j)/(image_height-1), 0.25);
         compgeom::write_color(outfile, pixel_color);

         image[(j*image_width*n_channels) + (n_channels*i) + 0] = static_cast<char>(255.999 * pixel_color.x());
         image[(j*image_width*n_channels) + (n_channels*i) + 1] = static_cast<char>(255.999 * pixel_color.y());
         image[(j*image_width*n_channels) + (n_channels*i) + 2] = static_cast<char>(255.999 * pixel_color.z());
      }
   }

   auto err = stbi_write_png("image.png",image_width,image_height,n_channels,image,(image_width * n_channels));

}