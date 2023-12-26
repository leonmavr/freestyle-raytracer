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
            // TODO: sub -H/2, -W/2
            end = (vec3_i32_t) {c, r, plane_z};
            ray_set(&ray, &ray.origin, &end);
            // TODO: make vector to end
            //ray_set(&ray, &ray.origin, )
            (*canvas)[r+HEIGHT/2][c+WIDTH/2].x = (*bg)[r+HEIGHT/2][c+WIDTH/2].x; 
            (*canvas)[r+HEIGHT/2][c+WIDTH/2].y = (*bg)[r+HEIGHT/2][c+WIDTH/2].y; 
            (*canvas)[r+HEIGHT/2][c+WIDTH/2].z = (*bg)[r+HEIGHT/2][c+WIDTH/2].z; 
            // paint intersections red
            if (ray_sphere_inters(&ray, sph).z != 0) {
                vec3_u8_t color = sphere_reflect(sph, &ray);
                (*canvas)[r+HEIGHT/2][c+WIDTH/2].x = color.x;
                (*canvas)[r+HEIGHT/2][c+WIDTH/2].y = color.y;
                (*canvas)[r+HEIGHT/2][c+WIDTH/2].z = color.z;
            }
        }
    }
}

