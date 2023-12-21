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
    vec3_u8_t col_grass = {50, 220, 30};
    vec3_u8_add(&col_bottom, &col_top);
    bg_create(&bg, &col_top, &col_bottom, &col_grass);
    //write_ppm(img);
    sphere_t sph;
    rt_run(&canvas, &sph, &bg);
    image_free(bg);
    image_free(canvas);
    printf("done\n");

    return 0;
}
