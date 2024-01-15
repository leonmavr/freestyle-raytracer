#include "types.h" 
#include "ut.h" 
#include "rt2.h" 
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    image_t bg = image_new();
    image_t canvas = image_new();
    vec3_u8_t col_top = {255, 255, 255};
    vec3_u8_t col_bottom = {100, 150, 200};
    bg_create(&bg, &col_top, &col_bottom);
    sphere_t sph = (sphere_t) {(vec3_i32_t) {0, 0, 400}, 105};
    cam_t cam;
#if 0
    lights_t lights;
    vec3_i32_t v = (vec3_i32_t) {500, 100, 50};
    vec3_i32_t u = (vec3_i32_t) {500, 100, 50};
    light_add(&lights, LIGHT_DIR, 0.5, &u);
    light_add(&lights, LIGHT_DIR, 0.5, &v);
    light_add(&lights, LIGHT_AMB, 0.1, &v);
    rt_run(&lights, &canvas, &sph, &bg, &cam);
#else
    cam_init(&cam, 0, 0, 100, 60);
    rt_run(NULL, &canvas, &sph, &bg, &cam);
#endif
    write_ppm(canvas, "output.ppm");
    image_free(bg);
    image_free(canvas);
    printf("done\n");

    return 0;
}
