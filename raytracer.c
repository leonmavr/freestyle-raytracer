#include "raytracer.h" 
#include "objects.h" 
#include "types.h" 
#include "background.h" 

void rt_run(image_t* canvas, sphere_t* sph, image_t* bg) {
    for (int r = 0; r < HEIGHT; ++r) {
        for (int c = 0; c < WIDTH; ++c) {
            (*canvas)[r][c].x = (*bg)[r][c].x; 
            (*canvas)[r][c].y = (*bg)[r][c].y; 
            (*canvas)[r][c].z = (*bg)[r][c].z; 
        }
    }
}
