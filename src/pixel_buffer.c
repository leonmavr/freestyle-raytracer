#include "pixel_buffer.h"

uint8_t pbuffer[PBUFFER_HEIGHT][PBUFFER_WIDTH][3];


void pbuffer_write(int x, int y, unsigned char r, unsigned char g, unsigned char b) {
    pbuffer[y][x][0] = r;
    pbuffer[y][x][1] = g;
    pbuffer[y][x][2] = b;
}


void pbuffer_save_ppm(const char* filepath) {
    FILE* fout = fopen(filepath, "wb");
    if (fout == NULL) {
        perror("Error opening output file.\n");
        return;
    }
    fprintf(fout, "P3\n%d %d\n255\n", PBUFFER_WIDTH, PBUFFER_HEIGHT);
    
    for (int r = 0; r < PBUFFER_HEIGHT; ++r) {
        for (int c = 0; c < PBUFFER_WIDTH; ++c) {
            fprintf(fout, "%u %u %u ", pbuffer[r][c][0], pbuffer[r][c][1], pbuffer[r][c][2]);
        }
        fprintf(fout, "\n");
    }
    fclose(fout);
}
