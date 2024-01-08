#ifndef OBJECTS_H
#define OBJECTS_H 

#include "types.h" 
#include <stdbool.h>
#include <stddef.h> // size_t

typedef struct ray_t {
    // direction unit vector
    vec3_f_t dir;
    vec3_i32_t origin;
} ray_t;

typedef struct sphere_t {
    vec3_i32_t origin;
    i32_t rad;
    float coeff_diffuse; // diffuse coefficient from 0 to 1
    vec3_i32_t color;
    i32_t color_range;
} sphere_t;

vec3_f_t sphere_unit_normal(sphere_t* sph, vec3_i32_t* where, bool normalise);

typedef struct camera_t {
    i32_t cx; // centre x
    i32_t cy; // centre y
    i32_t f; // focal length
    float fovx_rad; // field of view along x axis (in rads)
    float fovy_rad; // field of view along y ayis (in rads)
} camera_t;

typedef enum {
    LIGHT_AMB = 0, // ambient
    LIGHT_POINT,   // point
    LIGHT_DIR,     // directional
} light_type_t;

/** 
 * Light source model.
 * Each light source can have the following properties:
 * - type: identifier
 *     ambient/point/directional
 * - intensity: float
 *     0 to 1
 * - position: vec3  (only if type is point)
 * - direction: vec3  (only if type is directional)
 *
 * References:
 *   https://gabrielgambetta.com/computer-graphics-from-scratch/03-light.html
 */
typedef struct light_t {
    light_type_t type;
    float intensity; // intensity contribution
    union {
        vec3_i32_t dir; // direction in (3D) scene
        vec3_i32_t point; // point line (3D)
    } geometry;
} light_t;

/**
 * Collection of light sources. 
 */
typedef struct lights_t {
    // lights (ambient/directional/point) light sources
    light_t** lights;
    // number of lights
    size_t n;
} lights_t;

void light_add(lights_t* lights, light_type_t type, float intensity, vec3_i32_t* vec_descr);

vec3_i32_t ray_at(ray_t* ray, float t);

void ray_set(ray_t* ray, vec3_i32_t* start, vec3_i32_t* end);

void cam_set(camera_t* cam, i32_t cx, i32_t cy, i32_t f, float fov_deg);

bool cam_is_visible(camera_t* cam, vec3_i32_t* p);

bool ray_sphere_inters(ray_t* ray, sphere_t* sph, vec3_i32_t* where);

float light_compute_lights(lights_t* lights, vec3_i32_t* point, vec3_f_t* normal);

#endif /* OBJECTS_H */
