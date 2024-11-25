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
dbuffer_entry_t** dbuffer;

vec3f_t camera_project(vec3f_t xyz, bool* is_visible) {
    const float cx = camera.cx, cy = camera.cy, f = camera.f;
    // negate x and y to avoid inverted projections
    vec3f_t projected = (vec3f_t) {f*xyz.x/xyz.z - cx, f*xyz.y/xyz.z - cy, -camera.f};
    *is_visible =
        (projected.x > camera.boundary.x0 && projected.x < camera.boundary.x1) &&
        (projected.y > camera.boundary.y0 && projected.y < camera.boundary.y1);
    return projected;
}


void camera_init(float cx, float cy, float f, float fovx_deg, float fovy_deg) {
    // initialize camera
    camera.init = camera_init;
    camera.project = camera_project;
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
    dbuffer = malloc(height * sizeof(dbuffer_entry_t *));
    // TODO: check if alloc failed - pbuffer and pbuffer[0]
    for (int i = 0; i < height; i++)
        dbuffer[i] = malloc(width * sizeof(dbuffer[0]));
    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            dbuffer[row][col] = (dbuffer_entry_t) {.color = (vec3u8_t) {0, 0, 0},
                                                   .depth = FLT_MAX};
        }
    }
}

void camera_free() {
    int height = camera.boundary.y1 - camera.boundary.y0;
    for (int i = 0; i < height; ++i)
        free(dbuffer[i]);
    free(dbuffer);   
}

void dbuffer_write(int x, int y, uint8_t r, uint8_t g, uint8_t b, float dist) {
    // as 0x00RRGGBB
    const uint32_t color = (r << 16) | (g << 8) | b;
    const int width = camera.boundary.x1 - camera.boundary.x0;
    const int height = camera.boundary.y1 - camera.boundary.y0;
    // map from camera plane to 2D array indexes
    int x_idx = lmap_float(x, camera.boundary.x0, camera.boundary.x1, 0, width - 1);
    int y_idx = lmap_float(y, camera.boundary.y0, camera.boundary.y1, 0, height - 1);
    dbuffer[y_idx][x_idx] = (dbuffer_entry_t) {.color = (vec3u8_t) {r, g, b},
                                               .depth = dist}; 
}
