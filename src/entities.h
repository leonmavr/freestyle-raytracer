#ifndef ENTITIES_H
#define ENTITIES_H

#include "vmath.h"
#include <stdbool.h> 

typedef struct {
    float cx, cy;  // camera's center of perspective
    float f;       // focal length
    struct {
        float x0, y0, x1, y1;
    } boundary;
    vec3f_t (*project)(vec3f_t xyz, bool* is_visible);
    void (*init)(float cx, float cy, float f, float fovx_deg, float fovy_deg);
} camera_t;

extern camera_t camera;

typedef struct {
    vec3f_t origin;  
    float rad;
} sphere_t;

typedef struct { vec3f_t dir, origin; } ray_t;

void camera_init(float cx, float cy, float f, float fovx_deg, float fovy_deg);
vec3f_t ray_at(ray_t ray, float t);
ray_t ray_get(vec3f_t begin, vec3f_t end);
vec3i32_t cam2pbuffer(vec3f_t proj);
vec3u8_t intersect_sphere(ray_t ray, sphere_t sphere, bool* does_intersect);

#endif // ENTITIES_H
