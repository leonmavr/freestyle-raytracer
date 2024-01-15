
#include "types.h"
#include <stdlib.h>
#include <stdio.h>

/**
* @brief Write some RGB image data as a .ppm file.
*        A ppm image file looks as follows:
*
*        P3
*        # that's a comment
*        # P3 means colours are 8-bit
*        # c = columns, r = rows
*        # m = max colour, e.g. 255
*        c r
*        m
*        # below is the data section in rows with rgb triplets
*        r1 g1 b1 r2 g2 b2 ...
*        .               .
*        .               .
*        .               .
*        rm gm bm rn gn bn
*
* @param image The raw data as a 2D matrix of rgb values
* @param path  The path to save the image to
*/
void write_ppm(image_t image, const char* path) {
    FILE* fout;
    fout = fopen(path, "wb");
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
            fprintf(fout, "%u %u %u ", image[r][c].x, image[r][c].y,
                                       image[r][c].z);
        }
        fprintf(fout, "\n");
    }
}


