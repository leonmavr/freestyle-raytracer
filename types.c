#include "types.h" 
#include <stdlib.h>
#include <stdio.h>

image_t image_new() {
    vec3_u8_t** ret = malloc(HEIGHT * sizeof(vec3_u8_t*));
    for (int i = 0; i < HEIGHT; ++i) {
        ret[i] = malloc(sizeof(vec3_u8_t) * WIDTH);
        for (int j = 0; j < WIDTH; ++j) {
            ret[i][j].x = 0;
            ret[i][j].y = 0;
            ret[i][j].z = 0;
        }
    }
    return ret;
}

void image_free(image_t img) {
    for (int i = 0; i < HEIGHT; ++i)
        free(img[i]);
    free(img);
}

DEFINE_VEC_ADD(u8)
DEFINE_VEC_ADD(i32)
DEFINE_VEC_ADD(f)
DEFINE_VEC_SUB(u8)
DEFINE_VEC_SUB(i32)
DEFINE_VEC_SUB(f)
DEFINE_VEC_SCALMUL(u8)
DEFINE_VEC_SCALMUL(i32)
DEFINE_VEC_SCALMUL(f)
DEFINE_VEC_DOT(u8)
DEFINE_VEC_DOT(i32)
DEFINE_VEC_DOT(f)
DEFINE_VEC_NORM(u8)
DEFINE_VEC_NORM(i32)
DEFINE_VEC_NORM(f)
DEFINE_VEC_GET_UNIT(u8)
DEFINE_VEC_GET_UNIT(i32)
DEFINE_VEC_GET_UNIT(f)
