#include "entities.h"
#include "vmath.h"
#include "pixel_buffer.h"
#include "utils.h"
#include <math.h> 
#include <stdbool.h> 


#define PI 3.141592653589
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define ABS(a) ((a) > 0 ? (a) : -(a))
#define DEG2RAD(deg) ((deg) * PI / 180.0)

camera_t camera;

vec3f_t camera_project(vec3f_t xyz, bool* is_visible) {
    const float cx = camera.cx, cy = camera.cy, f = camera.f;
    // negate x and y to avoid inverted projections
    vec3f_t projected = (vec3f_t) {-f*xyz.x/xyz.z + cx, -f*xyz.y/xyz.z + cy, -camera.f};
    *is_visible =
        (projected.x > camera.boundary.x0 && projected.x < camera.boundary.x1) &&
        (projected.y > camera.boundary.y0 && projected.y < camera.boundary.y1);
    return projected;
}


void camera_init(float cx, float cy, float f, float fovx_deg, float fovy_deg) {
    camera.init = camera_init;
    camera.project = camera_project;
    camera.cx = cx;
    camera.cy = cy;
    camera.f = f;
    camera.boundary.x0 = MIN(f*tan(DEG2RAD(fovx_deg/2)) + cx, f*tan(DEG2RAD(-fovx_deg/2)) - cx);
    camera.boundary.x1 = MAX(f*tan(DEG2RAD(fovx_deg/2)) + cx, f*tan(DEG2RAD(-fovx_deg/2)) - cx);
    camera.boundary.y0 = MIN(f*tan(DEG2RAD(fovy_deg/2)) + cy, f*tan(DEG2RAD(-fovy_deg/2)) - cy);
    camera.boundary.y1 = MAX(f*tan(DEG2RAD(fovy_deg/2)) + cy, f*tan(DEG2RAD(-fovy_deg/2)) - cy);
}


vec3f_t ray_at(ray_t ray, float t) {
    return (vec3f_t) {ray.origin.x + t*ray.dir.x,
                      ray.origin.y + t*ray.dir.y,
                      ray.origin.z + t*ray.dir.z};
}

ray_t ray_get(vec3f_t begin, vec3f_t end) {
    ray_t ret;
    ret.origin = begin;
    ret.dir = vec3f_unit(vec3f_sub(end,begin)); 
    return ret;
}

vec3i32_t cam2pbuffer(vec3f_t proj) {
    int px = (int)lerp(0, PBUFFER_WIDTH - 1,
                       (proj.x - camera.boundary.x0) / (camera.boundary.x1 - camera.boundary.x0));
    int py = (int)lerp(0, PBUFFER_HEIGHT - 1,
                       (proj.y - camera.boundary.y0) / (camera.boundary.y1 - camera.boundary.y0));
    return (vec3i32_t) {px, py, 0};
}
