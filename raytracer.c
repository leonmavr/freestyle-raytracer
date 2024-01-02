#include "raytracer.h" 
#include "objects.h" 
#include "types.h" 
#include "background.h" 
#include <stdio.h>

void rt_run(image_t* canvas, sphere_t* sph, image_t* bg, camera_t* cam) {
    ray_t ray;
    float theta = 0; // TODO camera fov
    // the plane where to send the rays to
    i32_t plane_z = (float)HEIGHT/2/tan(cam->fovy_rad/2);
    // where to send the ray
    vec3_i32_t end;
    // z coordinate of fov along x and y axes
    for (int r = -HEIGHT/2; r < HEIGHT/2; ++r) {
        for (int c = -WIDTH/2; c < WIDTH/2; ++c) {
            end = (vec3_i32_t) {c, r, plane_z};
            ray_set(&ray, &ray.origin, &end);
            // 2D matrix indexes
            const size_t ir = r + HEIGHT/2, ic = c + WIDTH/2;
            (*canvas)[ir][ic].x = (*bg)[ir][ic].x; 
            (*canvas)[ir][ic].y = (*bg)[ir][ic].y; 
            (*canvas)[ir][ic].z = (*bg)[ir][ic].z; 
            // paint intersections red
            vec3_i32_t where;
            if (ray_sphere_inters(&ray, sph, &where) != 0) {
                (*canvas)[ir][ic].x = 255;
                (*canvas)[ir][ic].y = 0;
                (*canvas)[ir][ic].z = 0;
            }
        }
    }
}

