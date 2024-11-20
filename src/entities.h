#ifndef ENTITIES_H
#define ENTITIES_H

#include "vmath.h"
//#include "entities.h"
#include <stdbool.h> 
#include <stdio.h> 

#define LIGHTS_CAPACITY 25 

typedef struct {
    float cx, cy;  // camera's center of perspective
    float f;       // focal length
    struct {
        float x0, y0, x1, y1;
    } boundary;
    // 2D pixel buffer array with bottom 3 bytes of each entry corresponding to RGB
    vec3f_t (*project)(vec3f_t xyz, bool* is_visible);
    void (*init)(float cx, float cy, float f, float fovx_deg, float fovy_deg);
} camera_t;

extern uint32_t** cam_pbuffer;
void cam_pbuffer_init();
void cam_pbuffer_write(int x, int y, uint8_t r, uint8_t g, uint8_t b);
void cam_pbuffer_save(const char* filename);
void cam_pbuffer_free();

extern camera_t camera;

typedef struct {
    vec3f_t origin;  
    float rad;
    vec3u8_t color;
    float specular; // low values like 10 - matte, high light 1000 - shiny
} sphere_t;

typedef struct { vec3f_t dir, origin; } ray_t;

void camera_init(float cx, float cy, float f, float fovx_deg, float fovy_deg);
vec3f_t ray_at(ray_t ray, float t);
ray_t ray_get(vec3f_t begin, vec3f_t end);
vec3i32_t cam2pbuffer(vec3f_t proj);
vec3u8_t hit_sphere(ray_t ray, sphere_t sphere, bool* does_intersect);

typedef enum {
    LIGHT_AMB = 0,  // ambient
    LIGHT_POINT,    // point
    LIGHT_DIR,      // directional
} light_type_t;


/** 
 * Light source model.
 * There are three types of lights:
 * -- ambient - modelled as additive
 *    Has:
 *        - intensity
 * -- positional - all originate from the same point
 *    Has:
 *        - intensity
 *        - position
 * -- directional - all have the same direction
 *    Has:
 *        - intensity
 *        - direction 
 * All intensities must sum to 1 to avoid overexposure.
 *
 * References:
 * https://gabrielgambetta.com/computer-graphics-from-scratch/03-light.html
 */
typedef struct light_t {
    light_type_t type;
    float intensity;     // normalised intensity contribution
    union {
        vec3f_t dir;     // unit vector 
        vec3f_t point;
    };
} light_t;

typedef struct {
    light_t light[LIGHTS_CAPACITY];  // fixed-size array of lights
    size_t count;                    // number of lights currently in the scene
    // defines each light in the scene
    struct {
        void (*ambient_light)(float intensity);
        void (*point_light)(float intensity, float posx, float poxy, float posz);
        void (*dir_light)(float intensity, float dirx, float diry, float dirz);
    } add;
    void (*init)(void);
    void (*normalize)(void);
} lights_t;

void lights_init(void);

extern lights_t lights;

#endif // ENTITIES_H
