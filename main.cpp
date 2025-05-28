#include "tgaimage.h"
#include <cassert>
#include <iostream>
#include <cmath>
#include <string>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include "vec.h"
#include "gl.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green = TGAColor(0, 255,   0,   255);
const TGAColor blue  = TGAColor(0, 0,   255,   255);

int main(int argc, char** argv) {
    int width, height;
    
    width = 1024;
    height = 1024;
    uint32_t offset, scale_factor;
    scale_factor = 300;
    offset = 1024 / 2;

	TGAImage image(width, height, TGAImage::RGB);


    

    int zbuffer[width*height];
    for(int x=0; x<width; ++x) {
        for(int y = 0; y < height; ++y) {
            zbuffer[x*width + y] = std::numeric_limits<int>::min();
        }
    }


    // wireframe_render("african_head.obj", scale_factor, offset, image, white);

    solid_render("diablo3_pose.obj", "diablo3_pose_diffuse.tga",scale_factor, offset, image, white, width, zbuffer);


	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
	return 0;
}

