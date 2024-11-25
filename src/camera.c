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

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define PI 3.141592653589
#define DEG2RAD(deg) ((deg) * PI / 180.0)

camera_t camera;
uint32_t** cam_pbuffer;

vec3f_t camera_project(vec3f_t xyz, bool* is_visible) {
    const float cx = camera.cx, cy = camera.cy, f = camera.f;
    // negate x and y to avoid inverted projections
    vec3f_t projected = (vec3f_t) {f*xyz.x/xyz.z - cx, f*xyz.y/xyz.z - cy, -camera.f};
    *is_visible =
        (projected.x > camera.boundary.x0 && projected.x < camera.boundary.x1) &&
        (projected.y > camera.boundary.y0 && projected.y < camera.boundary.y1);
    return projected;
}


static void cam_pbuffer_init() {
    //// allocate pixel buffer matrix
    const int width = camera.boundary.x1 - camera.boundary.x0;
    const int height = camera.boundary.y1 - camera.boundary.y0;
    // allocate the pixel buffer - bottom 3 bytes of each element correspond to RGB
    cam_pbuffer = malloc(height * sizeof(uint32_t *));
    // TODO: check if alloc failed - pbuffer and pbuffer[0]
    for (int i = 0; i < height; i++)
        cam_pbuffer[i] = malloc(width * sizeof(cam_pbuffer[0]));
}

void camera_init(float cx, float cy, float f, float fovx_deg, float fovy_deg) {
    camera.init = camera_init;
    camera.project = camera_project;
    camera.cx = cx;
    camera.cy = cy;
    camera.f = f;
    camera.boundary.x0 = MIN(f*tan(DEG2RAD(fovx_deg/2)) + cx, f*tan(DEG2RAD(-fovx_deg/2)) + cx);
    camera.boundary.x1 = MAX(f*tan(DEG2RAD(fovx_deg/2)) + cx, f*tan(DEG2RAD(-fovx_deg/2)) + cx);
    camera.boundary.y0 = MIN(f*tan(DEG2RAD(fovy_deg/2)) + cy, f*tan(DEG2RAD(-fovy_deg/2)) + cy);
    camera.boundary.y1 = MAX(f*tan(DEG2RAD(fovy_deg/2)) + cy, f*tan(DEG2RAD(-fovy_deg/2)) + cy);
    cam_pbuffer_init();
}

void cam_pbuffer_write(int x, int y, uint8_t r, uint8_t g, uint8_t b) {
    // as 0x00RRGGBB
    const uint32_t color = (r << 16) | (g << 8) | b;
    const int width = camera.boundary.x1 - camera.boundary.x0;
    const int height = camera.boundary.y1 - camera.boundary.y0;
    // map from camera plane to 2D array indexes
    int x_idx = lmap_float(x, camera.boundary.x0, camera.boundary.x1, 0, width - 1);
    int y_idx = lmap_float(y, camera.boundary.y0, camera.boundary.y1, 0, height - 1);
    cam_pbuffer[y_idx][x_idx] = color;
}

vec3i32_t cam2pbuffer(vec3f_t proj) {
    int px = lmap(proj.x, camera.boundary.x0, camera.boundary.x1, 0, PBUFFER_WIDTH);
    int py = lmap(proj.x, camera.boundary.y0, camera.boundary.y1, 0, PBUFFER_HEIGHT);
    return (vec3i32_t) {px, py, 0};
}

void cam_pbuffer_save(const char* filename) {
    // open the file for writing
    FILE* ppm_file = fopen(filename, "wb");
    if (ppm_file == NULL) {
        perror("Error opening output file.\n");
        return;
    }
    const int height = camera.boundary.y1 - camera.boundary.y0;
    const int width = camera.boundary.x1 - camera.boundary.x0;
    fprintf(ppm_file, "P3\n%d %d\n255\n", width, height);
    // write the pixel buffer into the file
    for (int r = 0; r < height; ++r) {
        for (int c = 0; c < width; ++c) {
            uint32_t color = cam_pbuffer[r][c];
            uint8_t b = color & 0xff, g = (color >> 8) & 0xff, r = (color >> 16) & 0xff;
            fprintf(ppm_file, "%u %u %u ", r, g, b);
        }
        fprintf(ppm_file, "\n");
    } 
    printf("Saved ray tracing output as %s.\n", filename);
    fclose(ppm_file);
}


void cam_pbuffer_free() {
    int height = camera.boundary.y1 - camera.boundary.y0;
    for (int i = 0; i < height; ++i)
        free(cam_pbuffer[i]);
    free(cam_pbuffer);   
}

