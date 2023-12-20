#include "types.h" 
#include <stdlib.h>

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
