#include "pixel_buffer.h"
#include "camera.h"
#include "utils.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>  // fprintf

uint32_t** pbuffer;

void pbuffer_write(int x, int y, uint8_t r, uint8_t g, uint8_t b) {
    // as 0x00RRGGBB
    const uint32_t color = (r << 16) | (g << 8) | b;
    const int width = camera.boundary.x1 - camera.boundary.x0;
    const int height = camera.boundary.y1 - camera.boundary.y0;
    // map from camera plane to 2D array indexes
    int x_idx = lmap_float(x, camera.boundary.x0, camera.boundary.x1, 0, width - 1);
    int y_idx = lmap_float(y, camera.boundary.y0, camera.boundary.y1, 0, height - 1);
    pbuffer[y_idx][x_idx] = color;
}

void pbuffer_save(const char* filename) {
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
            uint32_t color = pbuffer[r][c];
            uint8_t b = color & 0xff, g = (color >> 8) & 0xff, r = (color >> 16) & 0xff;
            fprintf(ppm_file, "%u %u %u ", r, g, b);
        }
        fprintf(ppm_file, "\n");
    } 
    printf("Saved ray tracing output as %s.\n", filename);
    fclose(ppm_file);
}


void pbuffer_free() {
    int height = camera.boundary.y1 - camera.boundary.y0;
    for (int i = 0; i < height; ++i)
        free(pbuffer[i]);
    free(pbuffer);   
}

void pbuffer_init() {
    //// allocate pixel buffer matrix
    const int width = camera.boundary.x1 - camera.boundary.x0;
    const int height = camera.boundary.y1 - camera.boundary.y0;
    // allocate the pixel buffer - bottom 3 bytes of each element correspond to RGB
    pbuffer = malloc(height * sizeof(uint32_t *));
    // TODO: check if alloc failed - pbuffer and pbuffer[0]
    for (int i = 0; i < height; i++)
        pbuffer[i] = malloc(width * sizeof(pbuffer[0]));
}
