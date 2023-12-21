#ifndef OBJECTS_H
#define OBJECTS_H 

#include "types.h" 

typedef struct ray_t {
    // unit direction vector
    vec3_f_t dir;
    vec3_i32_t origin;
} ray_t;

typedef struct sphere_t {
    vec3_i32_t origin;
    i32_t rad;
} sphere_t;

vec3_i32_t ray_at(ray_t* ray, float t);

void ray_set(ray_t* ray, vec3_i32_t* start, vec3_i32_t* end);

vec3_i32_t ray_sphere_inters(ray_t* ray, sphere_t* sph);

#endif /* OBJECTS_H */
