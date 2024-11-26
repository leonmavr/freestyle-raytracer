#include "renderer.h"
#include "entities.h"
#include "camera.h"
#include "pixel_buffer.h"
#include <stddef.h>
#include <stdbool.h>
#include <float.h>

void render_objects(sphere_t* spheres, size_t num_spheres) {
    for (int y = camera.boundary.y0; y < camera.boundary.y1; ++y) {
        for (int x = camera.boundary.x0; x < camera.boundary.x1; ++x) {
            vec3i32_t proj_point = {x, y, -camera.f};
            vec3i32_t cam_origin = {camera.cx, camera.cy, 0};
            // "unproject" pixels by casting rays from the camera plane to its origin
            ray_t ray = ray_get(vec3ito3f(proj_point), vec3ito3f(cam_origin));

            // from ray's origin to closest hit
            float dist_closest = FLT_MAX;
            vec3u8_t color_closest = {0, 0, 0};
            for (size_t i = 0; i < num_spheres; ++i) {
                bool does_intersect = false;
                float dist_hit;
                vec3u8_t color = hit_sphere(ray, spheres[i], 
                                            &does_intersect, 
                                            &dist_hit);
                if (does_intersect && dist_hit < dist_closest) {
                    // if we have a hit, write current color to pixel buffer,
                    // update min. depth in depth buffer
                    dist_closest = dist_hit;
                    color_closest = color;
                    dbuffer_write(x, y, dist_closest);
                    pbuffer_write(x, y, color_closest.x, color_closest.y, color_closest.z);
                }
            }
        }
    }
}

