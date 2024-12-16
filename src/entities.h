#ifndef ENTITIES_H
#define ENTITIES_H

#include "vmath.h"
#include <stdbool.h> 
#include <stdio.h> 

#define MAX_BOUNCES 3

typedef struct { vec3f_t dir, origin; unsigned bounces; } ray_t;
typedef struct {
    vec3f_t origin;  
    float rad;
    vec3u8_t color;
    float specular;        // specular = 10 -> matte, specular = 100 -> shiny
    float reflective;      // 0 -> non reflective, 1 -> mirror
    unsigned long long id; // unique ID
} sphere_t;

sphere_t sphere_make(vec3f_t origin, float rad, vec3u8_t color, float specular, float reflective);
ray_t ray_get(vec3f_t begin, vec3f_t end);
ray_t hit_sphere(ray_t* ray, sphere_t sphere, bool* does_intersect,
                    float* dist, sphere_t* spheres, int num_spheres,
                    vec3u8_t* color);

#endif // ENTITIES_H
