#include "ppm_writer.h" 
#include "types.h" 
#include "background.h" 
#include "raytracer.h" 
#include "objects.h" 
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    image_t bg = image_new();
    image_t canvas = image_new();
    vec3_u8_t col_top = {255, 255, 255};
    vec3_u8_t col_bottom = {100, 150, 200};
    vec3_u8_t col_grass = {220, 100, 30};
    vec3_u8_add(&col_bottom, &col_top);
    bg_create(&bg, &col_top, &col_bottom, &col_grass);
    camera_t cam;
    cam_set(&cam, 0, 0, 100, 60);
    sphere_t sph = (sphere_t) {(vec3_i32_t) {80, -40, 400}, 105};
    lights_t lights;
    vec3_i32_t v = (vec3_i32_t) {50, -22, 44};
    vec3_i32_t u = (vec3_i32_t) {50, -12, 40};
    light_add(&lights, LIGHT_DIR, 0.3, &u);
    rt_run(&lights, &canvas, &sph, &bg, &cam);
    write_ppm(canvas);
    image_free(bg);
    image_free(canvas);
    printf("done\n");

    return 0;
}
