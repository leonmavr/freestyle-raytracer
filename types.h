#ifndef TYPES_H
#define TYPES_H 

#include <stdint.h>
#include <math.h>


#ifndef HEIGHT
#define HEIGHT 400
#endif

#ifndef WIDTH
#define WIDTH 600
#endif

/**
 * generic swap - swap any two data
 *
 * Works ONLY with gcc extensions
 * credits @Paul R (https://stackoverflow.com/a/3982397)
 */
#define SWAP(x, y) do {                                             \
    typeof(x) ret = x;                                              \
    x = y;                                                          \
    y = ret;                                                        \
} while (0)


typedef unsigned char u8_t;
typedef int32_t i32_t;

typedef struct vec2_i32_t {
    i32_t x,y;
} vec2_i32_t;

typedef struct vec3_u8_t {
    u8_t x, y, z;
} vec3_u8_t;

typedef struct vec3_i32_t {
    i32_t x, y, z;
} vec3_i32_t;

typedef struct vec3_f_t {
    float x, y, z;
} vec3_f_t;

typedef vec3_u8_t** image_t;

image_t image_new();
void image_free(image_t img);

#define DECLARE_VEC_ADD(type)                                        \
    vec3_##type##_t vec3_##type##_add(vec3_##type##_t* v1,           \
                                      vec3_##type##_t* v2);

#define DECLARE_VEC_SUB(type)                                        \
    vec3_##type##_t vec3_##type##_sub(vec3_##type##_t* v1,           \
                                      vec3_##type##_t* v2);

#define DECLARE_VEC_SCALMUL(type)                                    \
    vec3_##type##_t vec3_##type##_scalmul(vec3_##type##_t* v1,       \
                                          float a);

#define DECLARE_VEC_DOT(type)                                        \
    float vec3_##type##_dot(vec3_##type##_t* v1,                     \
                                vec3_##type##_t* v2);

#define DECLARE_VEC_NORM(type)                                       \
    float vec3_##type##_norm(vec3_##type##_t* v1);

#define DECLARE_VEC_GET_UNIT(type)                                   \
    vec3_f_t vec3_##type##_get_unit(vec3_##type##_t* v1);
 

#define DEFINE_VEC_ADD(type)                                         \
    vec3_##type##_t vec3_##type##_add(vec3_##type##_t* v1,           \
                                      vec3_##type##_t* v2) {         \
        vec3_##type##_t ret;                                         \
        ret.x = v1->x + v2->x;                                       \
        ret.y = v1->y + v2->y;                                       \
        ret.z = v1->z + v2->z;                                       \
        return ret;                                                  \
    }

#define DEFINE_VEC_SUB(type)                                         \
    vec3_##type##_t vec3_##type##_sub(vec3_##type##_t* v1,           \
                                      vec3_##type##_t* v2) {         \
        vec3_##type##_t ret;                                         \
        ret.x = v2->x - v1->x;                                       \
        ret.y = v2->y - v1->y;                                       \
        ret.z = v2->z - v1->z;                                       \
        return ret;                                                  \
    }


#define DEFINE_VEC_SCALMUL(type)                                    \
    vec3_##type##_t vec3_##type##_scalmul(vec3_##type##_t* v1,      \
                                          float a) {                \
        vec3_##type##_t ret;                                        \
        ret.x = v1->x * a;                                          \
        ret.y = v1->y * a;                                          \
        ret.z = v1->z * a;                                          \
        return ret;                                                 \
    }

#define DEFINE_VEC_DOT(type)                                       \
    float vec3_##type##_dot(vec3_##type##_t* v1,                   \
                                vec3_##type##_t* v2) {             \
        return (float)v1->x*v2->x + v1->y*v2->y + v1->z*v2->z;     \
    }


#define DEFINE_VEC_NORM(type)                                      \
    float vec3_##type##_norm(vec3_##type##_t* v1) {                \
        const float xx = v1->x * v1->x;                            \
        const float yy = v1->y * v1->y;                            \
        const float zz = v1->z * v1->z;                            \
        return sqrt(xx + yy + zz);                                 \
    }

#define DEFINE_VEC_GET_UNIT(type)                                  \
    vec3_f_t vec3_##type##_get_unit(vec3_##type##_t* v1) {         \
        const float n = vec3_##type##_norm(v1);                    \
        return (vec3_f_t) {v1->x/n, v1->y/n, v1->z/n};             \
    }

DECLARE_VEC_ADD(u8)
DECLARE_VEC_ADD(i32)
DECLARE_VEC_ADD(f)
DECLARE_VEC_SUB(u8)
DECLARE_VEC_SUB(i32)
DECLARE_VEC_SUB(f)
DECLARE_VEC_SCALMUL(u8)
DECLARE_VEC_SCALMUL(i32)
DECLARE_VEC_SCALMUL(f)
DECLARE_VEC_DOT(u8)
DECLARE_VEC_DOT(i32)
DECLARE_VEC_DOT(f)
DECLARE_VEC_NORM(u8)
DECLARE_VEC_NORM(i32)
DECLARE_VEC_NORM(f)
DECLARE_VEC_GET_UNIT(u8)
DECLARE_VEC_GET_UNIT(i32)
DECLARE_VEC_GET_UNIT(f)


#endif // TYPES_H
