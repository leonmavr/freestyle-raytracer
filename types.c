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

DEFINE_VEC_ADD_FUNCTION(u8)
DEFINE_VEC_ADD_FUNCTION(i32)
DEFINE_VEC_ADD_FUNCTION(f)
DEFINE_VEC_SUB_FUNCTION(u8)
DEFINE_VEC_SUB_FUNCTION(i32)
DEFINE_VEC_SUB_FUNCTION(f)
DEFINE_VEC_SCALMUL_FUNCTION(u8)
DEFINE_VEC_SCALMUL_FUNCTION(i32)
DEFINE_VEC_SCALMUL_FUNCTION(f)
DEFINE_VEC_DOT_FUNCTION(u8)
DEFINE_VEC_DOT_FUNCTION(i32)
DEFINE_VEC_DOT_FUNCTION(f)
