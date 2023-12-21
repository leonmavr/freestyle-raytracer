#ifndef OBJECTS_H
#define OBJECTS_H 

#include "types.h" 

typedef struct ray_t {
    // unit direction vector
    vec3_f_t dir;
    vec3_u32_t origin;
} ray_t;

typedef struct sphere_t {
    vec3_u32_t origin;
    vec3_u32_t rad;
} sphere_t;

vec3_u32_t ray_at(ray_t* ray, float t);

#endif /* OBJECTS_H */
