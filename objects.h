#ifndef OBJECTS_H
#define OBJECTS_H 

#include "types.h" 
#include <stdbool.h>

typedef struct ray_t {
    // direction unit vector
    vec3_f_t dir;
    vec3_i32_t origin;
} ray_t;

typedef struct sphere_t {
    vec3_i32_t origin;
    i32_t rad;
} sphere_t;

typedef struct camera_t {
    i32_t cx; // centre x
    i32_t cy; // centre y
    i32_t f; // focal length
    float fovx_rad; // field of view along x axis (in rads)
    float fovy_rad; // field of view along y ayis (in rads)
} camera_t;

vec3_i32_t ray_at(ray_t* ray, float t);

void ray_set(ray_t* ray, vec3_i32_t* start, vec3_i32_t* end);

void cam_set(camera_t* cam, i32_t cx, i32_t cy, i32_t f, float fov_deg);

bool cam_is_visible(camera_t* cam, vec3_i32_t* p);

vec3_i32_t ray_sphere_inters(ray_t* ray, sphere_t* sph);

#endif /* OBJECTS_H */
