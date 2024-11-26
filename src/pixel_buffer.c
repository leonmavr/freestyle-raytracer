#include "pixel_buffer.h"
#include "camera.h"
#include "utils.h"
#include <stdlib.h>         // malloc
#include <stdint.h>         // uint8_t
#include <stdio.h>          // fprintf

uint32_t** pbuffer;

void pbuffer_write(int x, int y, uint8_t r, uint8_t g, uint8_t b) {
    // as 0x00RRGGBB
    const uint32_t color = (r << 16) | (g << 8) | b;
    // map from camera plane to 2D array indexes
    int x_idx = lmap_float(x, camera.boundary.x0, camera.boundary.x1,
                              0, camera.boundary.width - 1);
    int y_idx = lmap_float(y, camera.boundary.y0, camera.boundary.y1,
                              0, camera.boundary.height - 1);
    pbuffer[y_idx][x_idx] = color;
}

void pbuffer_save(const char* filename) {
    // open the file for writing
    FILE* ppm_file = fopen(filename, "wb");
    if (ppm_file == NULL) {
        perror("Error opening output file.\n");
        return;
    }
    fprintf(ppm_file, "P3\n%d %d\n255\n", camera.boundary.width, camera.boundary.height);
    // write the pixel buffer into the file
    for (int r = 0; r < camera.boundary.height; ++r) {
        for (int c = 0; c < camera.boundary.width; ++c) {
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
    for (int i = 0; i < camera.boundary.height; ++i)
        free(pbuffer[i]);
    free(pbuffer);   
}

void pbuffer_init() {
    //// allocate pixel buffer matrix
    // allocate the pixel buffer - bottom 3 bytes of each element correspond to RGB
    pbuffer = malloc(camera.boundary.height * sizeof(uint32_t *));
    // TODO: check if alloc failed - pbuffer and pbuffer[0]
    for (int i = 0; i < camera.boundary.height; i++)
        pbuffer[i] = malloc(camera.boundary.width * sizeof(pbuffer[0]));
}
