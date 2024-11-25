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
extern uint32_t** cam_pbuffer;

vec3i32_t cam2pbuffer(vec3f_t proj);
void cam_pbuffer_write(int x, int y, uint8_t r, uint8_t g, uint8_t b);
/**
 * Same the ppm buffer matrix as a file.
 * A ppm image file looks as follows:
 *
 * P3
 * # that's a comment
 * # P3 means colours are 8-bit
 * # c = columns, r = rows
 * # m = max colour, e.g. 255
 * c r
 * m
 * # below is the data section with rgb triplets
 * r g b r g b ... r g b
 * .                   .
 * .                   .
 * .                   .
 * r g b r g b ... r g b
 */
void cam_pbuffer_save(const char* filename);
void cam_pbuffer_free();
void camera_init(float cx, float cy, float f, float fovx_deg, float fovy_deg);

#endif // CAMERA_H
