#ifndef PIXEL_BUFFER_H 
#define PIXEL_BUFFER_H 

#include <stdint.h> // uint8_t

extern uint32_t** pbuffer;

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
void pbuffer_init();
void pbuffer_write(int x, int y, uint8_t r, uint8_t g, uint8_t b);
void pbuffer_save(const char* filename);
void pbuffer_free();

#endif // PIXEL_BUFFER_H 
