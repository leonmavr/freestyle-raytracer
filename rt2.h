#ifndef RT_H
#define RT_H 

#include "types.h" 
#include <stdbool.h>

#ifndef WIDTH
#define WIDTH 600
#endif

#ifndef HEIGHT 
#define HEIGHT 400 
#endif

typedef struct ray_t {
    // direction unit vector
    vec3_f_t dir;
    vec3_i32_t origin;
} ray_t;

typedef struct sphere_t {
    vec3_i32_t origin;
    i32_t rad;
    vec3_i32_t color;
} sphere_t;

typedef struct cam_t {
    i32_t cx;       // centre x
    i32_t cy;       // centre y
    i32_t f;        // focal length
    float fovx_rad; // field of view along x axis (in rads)
    float fovy_rad; // field of view along y ayis (in rads)
} cam_t;

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
    float intensity;      // intensity contribution
    union {
        vec3_i32_t dir;   // direction in (3D) scene
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

/**
* @brief Create a vertical linear gradient as the background 
*
* @param img        2D matrix of RGB values
* @param col_top    Colour at the top row of the background
* @param col_bottom Colour at the bottom row of the background
*/
void bg_create(image_t* img, vec3_u8_t* col_top, vec3_u8_t* col_bottom);

/**
* @brief Initialise pinhole camera
*
* @param cam camera object
* @param cx x of the centre
* @param cy y of the centre
* @param f  focal length
* @param fov_deg Field of view in degrees
*/
void cam_init(cam_t* cam, i32_t cx, i32_t cy, i32_t f, float fov_deg);

void cam_move(cam_t* cam, vec3_i32_t* xyz);

/**
* @brief Perspective transform for a pinhole camera
*
* @param cam Camera object
* @param pt 3D point object
*
* @returns 2D projected point 
*/
vec2_i32_t persp_transform(cam_t* cam, vec3_i32_t* pt);

vec3_i32_t ray_at(ray_t* ray, float t);

bool ray_sphere_inters(ray_t* ray, sphere_t* sph, vec3_i32_t* where);

void ray_set(ray_t* ray, vec3_i32_t* start, vec3_i32_t* end);

void rt_run(lights_t* lights, image_t* canvas, sphere_t* sph, image_t* bg, cam_t* cam);

#endif /* RT_H */
