#include "renderer.h"
#include "entities.h"
#include "camera.h"
#include "pixel_buffer.h"
#include <stddef.h>
#include <stdbool.h>
#include <float.h>

void render_objects(sphere_t* spheres, size_t num_spheres) {
    const int width = camera.boundary.x1 - camera.boundary.x0;
    const int height = camera.boundary.y1 - camera.boundary.y0;

    for (int y = camera.boundary.y0; y < camera.boundary.y1; ++y) {
        for (int x = camera.boundary.x0; x < camera.boundary.x1; ++x) {
            vec3i32_t proj_point = {x, y, -camera.f};
            vec3i32_t cam_origin = {camera.cx, camera.cy, 0};
            ray_t ray = ray_get(vec3ito3f(proj_point), vec3ito3f(cam_origin));

            float closest_depth = FLT_MAX; // Initialize closest depth to infinity
            vec3u8_t closest_color = {0, 0, 0}; // Default to black
            bool has_intersection = false;

            for (size_t i = 0; i < num_spheres; ++i) {
                bool does_intersect = false;
                float intersection_depth;
                vec3u8_t color = hit_sphere(ray, spheres[i], 
                                           &does_intersect, 
                                           &intersection_depth);

                if (does_intersect && intersection_depth < closest_depth) {
                    closest_depth = intersection_depth;
                    closest_color = color;
                    has_intersection = true;
                }
            }

            if (has_intersection) {
                // Update depth buffer and draw pixel
                //dbuffer[y][x] = (dbuffer_entry_t) {
                //    .color = closest_color,
                //    .depth = closest_depth
                //};
                dbuffer_write(x, y, closest_color.x, closest_color.y, closest_color.z, closest_depth);
                pbuffer_write(x, y, closest_color.x, closest_color.y, closest_color.z);
                printf("%.2f\n", closest_depth);
            }
        }
    }
}

