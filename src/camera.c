#include "camera.h"
#include "utils.h"
#include "vmath.h"
#include "pixel_buffer.h"
#include <stddef.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <float.h>

camera_t camera;
uint32_t** cam_pbuffer;
float** dbuffer;

void camera_init(float cx, float cy, float f, float fovx_deg, float fovy_deg) {
    // initialize camera
    camera.init = camera_init;
    camera.cx = cx;
    camera.cy = cy;
    camera.f = f;
    camera.boundary.x0 = UT_MIN(f*tan(UT_DEG2RAD(fovx_deg/2)) + cx, f*tan(UT_DEG2RAD(-fovx_deg/2)) + cx);
    camera.boundary.x1 = UT_MAX(f*tan(UT_DEG2RAD(fovx_deg/2)) + cx, f*tan(UT_DEG2RAD(-fovx_deg/2)) + cx);
    camera.boundary.y0 = UT_MIN(f*tan(UT_DEG2RAD(fovy_deg/2)) + cy, f*tan(UT_DEG2RAD(-fovy_deg/2)) + cy);
    camera.boundary.y1 = UT_MAX(f*tan(UT_DEG2RAD(fovy_deg/2)) + cy, f*tan(UT_DEG2RAD(-fovy_deg/2)) + cy);
    // initialize depth buffer
    const int width = camera.boundary.x1 - camera.boundary.x0;
    const int height = camera.boundary.y1 - camera.boundary.y0;
    // allocate the pixel buffer - bottom 3 bytes of each element correspond to RGB
    dbuffer = malloc(height * sizeof(float *));
    // TODO: check if alloc failed - pbuffer and pbuffer[0]
    for (int i = 0; i < height; i++)
        dbuffer[i] = malloc(width * sizeof(dbuffer[0]));
    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            dbuffer[row][col] = FLT_MAX;
        }
    }
}

void camera_free() {
    int height = camera.boundary.y1 - camera.boundary.y0;
    for (int i = 0; i < height; ++i)
        free(dbuffer[i]);
    free(dbuffer);   
}

void dbuffer_write(int x, int y, float dist) {
    const int width = camera.boundary.x1 - camera.boundary.x0;
    const int height = camera.boundary.y1 - camera.boundary.y0;
    int x_idx = lmap_float(x, camera.boundary.x0, camera.boundary.x1, 0, width - 1);
    int y_idx = lmap_float(y, camera.boundary.y0, camera.boundary.y1, 0, height - 1);
    dbuffer[y_idx][x_idx] = dist;
}
