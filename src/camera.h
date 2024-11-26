#ifndef CAMERA_H
#define CAMERA_H

#include "vmath.h"
#include <stdint.h>
#include <stdbool.h>

/** Pinhole camera */
typedef struct {
    float cx, cy;  // camera's center of perspective
    float f;       // focal length
    struct {
        float x0, y0, x1, y1;
        int width, height; // for easier referencing
    } boundary;    // projection plane boundaries
    void (*init)(float cx, float cy, float f, float fovx_deg, float fovy_deg);
} camera_t;

extern float** dbuffer;
extern camera_t camera;

void camera_init(float cx, float cy, float f, float fovx_deg, float fovy_deg);
void camera_free();
void dbuffer_write(int x, int y, float dist);

#endif // CAMERA_H
