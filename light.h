#ifndef LIGHT
#define LIGHT
#include "vec.h"
#include "tgaimage.h"

enum LightMode {
    point_light = 0,
    directional_light = 1,
    ambient_light = 2
};

class Light {
    private:
        TGAColor color;
        
        vec3 direction;
        vec3 position;

        LightMode mode;
        
        double intensity;
        double distance_falloff;

    public:
        
    Light();

    Light(vec3 light_direction, double light_intensity, TGAColor light_color);

    void set_color(TGAColor light_color);

    void set_direction(vec3 light_direction);

    void set_intensity(double light_intensity);

    void set_position(vec3 light_position);

    void set_mode(LightMode light_mode);

    void set_falloff(double light_falloff);

    TGAColor get_color();

    vec3 get_direction();

    double get_intensity();

    vec3 get_position();

    LightMode get_mode();

    double get_falloff();

    
};





#endif