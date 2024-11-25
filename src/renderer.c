#include "renderer.h"
#include "entities.h"
#include "camera.h"
#include "pixel_buffer.h"

void render_objects(sphere_t* sphere) {
    for (int y = camera.boundary.y0; y < camera.boundary.y1; ++y) {
        for (int x = camera.boundary.x0; x < camera.boundary.x1; ++x) {
            vec3i32_t proj_point = {x, y, -camera.f};
            vec3i32_t cam_origin = {camera.cx, camera.cy, 0};
            ray_t ray = ray_get(vec3ito3f(proj_point), vec3ito3f(cam_origin));
            bool does_intersect = false;
            vec3u8_t color = hit_sphere(ray, *sphere, &does_intersect);
            if (does_intersect)
                pbuffer_write(x, y, color.x, color.y, color.z); 
        }
    }
}
