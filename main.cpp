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
    


    Renderer renderer = Renderer();


    Model model = Model("obj/diablo3_pose.obj");

    renderer.load_texture("obj/diablo3_pose_diffuse.tga", 1024, 1024);

    renderer.load_image(1024, 1024);

    renderer.change_scale(300);

    // renderer.light.set_position({0, 500, 200});

    // renderer.change_rotation('y', 180, model);
    renderer.change_camera({0, 0});

    // renderer.wireframe(model, white);

    renderer.render(model);

    renderer.write("output.tga");
	return 0;
}

