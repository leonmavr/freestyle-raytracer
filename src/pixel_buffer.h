#ifndef PIXEL_BUFFER_H 
#define PIXEL_BUFFER_H 

#include <stdio.h>  // fprintf
#include <stdint.h> // uint8_t

#ifndef PBUFFER_HEIGHT
#define PBUFFER_HEIGHT 360
#endif

#ifndef PBUFFER_WIDTH 
#define PBUFFER_WIDTH 480 
#endif

/* pixel buffer */
extern uint8_t pbuffer[PBUFFER_HEIGHT][PBUFFER_WIDTH][3];

void pbuffer_write(int x, int y, unsigned char r, unsigned char g, unsigned char b);
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
 * r g b r g b ... r g b
 * .                   .
 * .                   .
 * .                   .
 * r g b r g b ... r g b
 */
void pbuffer_save_ppm(const char* filepath);

#endif // PIXEL_BUFFER_H 
