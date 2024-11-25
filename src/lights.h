#ifndef LIGHTS_H
#define LIGHTS_H

#include "vmath.h"
#include <stddef.h> // size_t

#define LIGHTS_CAPACITY 25 

typedef enum {
    LIGHT_AMB = 0,  // ambient
    LIGHT_POINT,    // point
    LIGHT_DIR,      // directional
} light_type_t;

/** 
 * Light source model.
 * There are three types of lights:
 * -- ambient - modelled as additive (reflection from the environment). Has:
 *        - intensity
 * -- positional - all originate from the same point (powerful flashlight). Has:
 *        - intensity
 *        - position
 * -- directional - all have the same direction (sunlight). Has:
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
        vec3f_t dir;     // must be aunit vector 
        vec3f_t point;
    };
} light_t;

/** Defines all the lights in the scene */
typedef struct {
    light_t light[LIGHTS_CAPACITY];  // up a a fixed number of light 
    size_t count;                    // number of lights currently in the scene
    // defines each light in the scene
    struct {
        void (*ambient_light)(float intensity);
        void (*point_light)(float intensity, float posx, float poxy, float posz);
        void (*dir_light)(float intensity, float dirx, float diry, float dirz);
    } add;
    void (*init)(void);
    /** Esnures all intensities sum up to 1 to avoid overexposure */
    void (*normalize)(void);
} lights_t;

extern lights_t lights;

void lights_init(void);

#endif // LIGHTS_H
