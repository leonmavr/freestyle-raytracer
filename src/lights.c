#include "lights.h"
#include "vmath.h"

lights_t lights;

static void lights_ambient_light(float intensity) {
    lights.light[lights.count].type = LIGHT_AMB;
    lights.light[lights.count++].intensity = intensity;
}

static void lights_point_light(float intensity, float posx, float posy, float posz) {
    lights.light[lights.count].type = LIGHT_POINT;
    lights.light[lights.count].intensity = intensity;
    lights.light[lights.count++].point = (vec3f_t) {posx, posy, posz};
}

static void lights_dir_light(float intensity, float dirx, float diry, float dirz) {
    lights.light[lights.count].type = LIGHT_DIR;
    lights.light[lights.count].intensity = intensity;
    //lights.light[lights.count++].dir = (vec3f_t) {dirx, diry, dirz};
    lights.light[lights.count++].dir = vec3f_unit((vec3f_t) {dirx, diry, dirz});
}

static void lights_normalize(void) {
    float total_intensity = .0;
    for (int i = 0; i < lights.count; ++i)
        total_intensity += lights.light[i].intensity;
    for (int i = 0; i < lights.count; ++i)
        lights.light[i].intensity /= total_intensity;
}

void lights_init(void) {
    //printf("L init done\n");
    lights.normalize = lights_normalize;
    lights.add.ambient_light = lights_ambient_light;    
    lights.add.dir_light = lights_dir_light;    
    lights.add.point_light = lights_point_light;    
    lights.count = 0;
}
