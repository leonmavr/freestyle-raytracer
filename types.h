#ifndef TYPES_H
#define TYPES_H 


#ifndef HEIGHT
#define HEIGHT 400
#endif

#ifndef WIDTH
#define WIDTH 600
#endif

typedef unsigned char u8_t;

typedef struct vec3_u8_t {
    u8_t x, y, z;
} vec3_u8_t;

typedef vec3_u8_t** image_t;

image_t image_new();
void image_free(image_t img);


#endif // TYPES_H
