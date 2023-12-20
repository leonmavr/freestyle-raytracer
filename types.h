#ifndef TYPES_H
#define TYPES_H 

#include <stdint.h>


#ifndef HEIGHT
#define HEIGHT 400
#endif

#ifndef WIDTH
#define WIDTH 600
#endif

typedef unsigned char u8_t;
typedef uint32_t u32_t;

typedef struct vec3_u8_t {
    u8_t x, y, z;
} vec3_u8_t;

typedef struct vec3_u32_t {
    u32_t x, y, z;
} vec3_u32_t;

typedef vec3_u8_t** image_t;

image_t image_new();
void image_free(image_t img);

#define DECLARE_VEC_ADD_FUNCTION(type)                               \
    vec3_##type##_t vec3_##type##_add(vec3_##type##_t* v1,           \
                                      vec3_##type##_t* v2);

#define DECLARE_VEC_SUB_FUNCTION(type)                               \
    vec3_##type##_t vec3_##type##_sub(vec3_##type##_t* v1,           \
                                      vec3_##type##_t* v2);

#define DECLARE_VEC_SCALMUL_FUNCTION(type)                           \
    vec3_##type##_t vec3_##type##_scalmul(vec3_##type##_t* v1,       \
                                          float a);

#define DEFINE_VEC_ADD_FUNCTION(type)                                \
    vec3_##type##_t vec3_##type##_add(vec3_##type##_t* v1,           \
                                      vec3_##type##_t* v2) {         \
        vec3_##type##_t ret;                                         \
        ret.x = v1->x + v2->x;                                       \
        ret.y = v1->y + v2->y;                                       \
        ret.z = v1->z + v2->z;                                       \
        return ret;                                                  \
    }

#define DEFINE_VEC_SUB_FUNCTION(type)                                \
    vec3_##type##_t vec3_##type##_sub(vec3_##type##_t* v1,           \
                                      vec3_##type##_t* v2) {         \
        vec3_##type##_t ret;                                         \
        ret.x = v2->x - v1->x;                                       \
        ret.y = v2->y - v1->y;                                       \
        ret.z = v2->z - v1->z;                                       \
        return ret;                                                  \
    }


#define DEFINE_VEC_SCALMUL_FUNCTION(type)                           \
    vec3_##type##_t vec3_##type##_scalmul(vec3_##type##_t* v1,      \
                                          float a) {                \
        vec3_##type##_t ret;                                        \
        ret.x = v1->x * a;                                          \
        ret.y = v1->y * a;                                          \
        ret.z = v1->z * a;                                          \
        return ret;                                                 \
    }

DECLARE_VEC_ADD_FUNCTION(u8);
DECLARE_VEC_ADD_FUNCTION(u32);
DECLARE_VEC_SUB_FUNCTION(u8);
DECLARE_VEC_SUB_FUNCTION(u32);
DECLARE_VEC_SCALMUL_FUNCTION(u8);
DECLARE_VEC_SCALMUL_FUNCTION(u32);


#endif // TYPES_H
