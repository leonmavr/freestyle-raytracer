#ifndef CAMERA_H
#define CAMERA_H

#include "vmath.h"
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    float cx, cy;  // camera's center of perspective
    float f;       // focal length
    struct {
        float x0, y0, x1, y1;
    } boundary;
    // 2D pixel buffer array with the 3 least significant bytes corresponding to RGB
    vec3f_t (*project)(vec3f_t xyz, bool* is_visible);
    void (*init)(float cx, float cy, float f, float fovx_deg, float fovy_deg);
} camera_t;

extern camera_t camera;

void camera_init(float cx, float cy, float f, float fovx_deg, float fovy_deg);


#endif // CAMERA_H
