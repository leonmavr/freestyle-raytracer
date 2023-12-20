#ifndef PPM_WRITER_H
#define PPM_WRITER_H 

#include "types.h"
#include "types.h" 
#include <stdlib.h>
#include <stdio.h>

#define PPM_OUTPUT_FILE "output.ppm"

/**
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
 * r1 g1 b1 r2 g2 b2 ...
 * .               .
 * .               .
 * .               .
 * rm gm bm rn gn bn
 */
void write_ppm(image_t image) {
    FILE* fout;
    fout = fopen(PPM_OUTPUT_FILE, "wb");
    if (fout == NULL) {
        perror("Error opening output file.\n");
        return;
    }
    // write the header to the .ppm file
    fprintf(fout, "P3\n");
    fprintf(fout, "%d %d\n", WIDTH, HEIGHT);
    fprintf(fout, "255\n");
    // write the data
    for (int r = 0; r < HEIGHT; ++r) {
        for (int c = 0; c < WIDTH; ++c) {
            fprintf(fout, "%u %u %u ", image[r][c].x, image[r][c].y, image[r][c].z);
        }
        fprintf(fout, "\n");
    }

}

#endif // PPM_WRITER_H
