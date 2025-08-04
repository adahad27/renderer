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
#include <SDL2/SDL.h>


const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green = TGAColor(0, 255,   0,   255);
const TGAColor blue  = TGAColor(0, 0,   255,   255);

SDL_Renderer *sdl_renderer = NULL;
SDL_Window *sdl_window = NULL;
SDL_Event event;

uint16_t width = 1024;
uint16_t height = 1024;

int main(int argc, char** argv) {
    

    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(width, height, 0, &sdl_window, &sdl_renderer);

    SDL_SetRenderDrawColor(sdl_renderer, 0, 0, 0, 0);
    SDL_RenderClear(sdl_renderer);

    SDL_SetRenderDrawColor(sdl_renderer, 255, 0, 0, 255);
    for (int i = 0; i < width; ++i)
        SDL_RenderDrawPoint(sdl_renderer, i, i);
    SDL_RenderPresent(sdl_renderer);
    while (1) {
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
            break;
    }
    SDL_DestroyRenderer(sdl_renderer);
    SDL_DestroyWindow(sdl_window);
    SDL_Quit();

    return 0;

    Renderer renderer = Renderer();


    Model model = Model("obj/diablo3_pose.obj");

    renderer.load_texture("obj/diablo3_pose_diffuse.tga", width, height);

    renderer.load_image(width, height);

    renderer.change_scale(300);

    renderer.light.set_direction({0, 0, 1});

    // renderer.change_rotation('y', 180, model);
    renderer.change_camera({0, 0});

    // renderer.wireframe(model, white);

    renderer.render(model);

    renderer.write("output.tga");
	return 0;
}

