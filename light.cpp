#include "light.h"
#include "vec.h"
void Light::set_color(TGAColor light_color) {
    color = light_color;
}

void Light::set_direction(vec3 light_direction) {
    direction = light_direction;
}

void Light::set_intensity(double light_intensity) {
    intensity = light_intensity;
}

void Light::set_position(vec3 light_position) {
    position = light_position;
}

void Light::set_mode(LightMode light_mode) {
    mode = light_mode;
}

void Light::set_falloff(double light_falloff) {
    distance_falloff = light_falloff;
}

Light::Light() {
    color = TGAColor(255, 255, 255, 255);
    
    direction = {0, 0, 1};
    position = {0, 0, 1};

    mode = directional_light;
    intensity = 1;
    distance_falloff = 200;
}

TGAColor Light::get_color() {
    return color;
}

vec3 Light::get_direction() {
    return direction;
}

double Light::get_intensity() {
    return intensity;
}

LightMode Light::get_mode() {
    return mode;
}

vec3 Light::get_position() {
    return position;
}

double Light::get_falloff() {
    return distance_falloff;
}