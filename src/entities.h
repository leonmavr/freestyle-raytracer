#ifndef ENTITIES_H
#define ENTITIES_H

#include "vmath.h"
#include <stdbool.h> 
#include <stdio.h> 



typedef struct {
    vec3f_t origin;  
    float rad;
    vec3u8_t color;
    float specular; // specular = 10 -> matte, specular = 100 -> shiny
} sphere_t;

typedef struct { vec3f_t dir, origin; } ray_t;
ray_t ray_get(vec3f_t begin, vec3f_t end);

vec3u8_t hit_sphere(ray_t ray, sphere_t sphere, bool* does_intersect);


#endif // ENTITIES_H
