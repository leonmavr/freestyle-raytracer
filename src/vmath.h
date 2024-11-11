#ifndef VMATH_H 
#define VMATH_H 

#include <stdint.h>
#include <math.h>

typedef unsigned char u8_t;
typedef int32_t i32_t;

typedef struct {
    u8_t x, y, z;
} vec3u8_t;

typedef struct {
    i32_t x, y, z;
} vec3i32_t;

typedef struct {
    float x, y, z;
} vec3f_t;


#define DECLARE_VEC_ADD_FUNCTION(type)                               \
    vec3##type##_t vec3##type##_add(vec3##type##_t v1,               \
                                      vec3##type##_t v2);

#define DECLARE_VEC_SUB_FUNCTION(type)                               \
    vec3##type##_t vec3##type##_sub(vec3##type##_t v1,               \
                                      vec3##type##_t v2);

#define DECLARE_VEC_SCALMUL_FUNCTION(type)                           \
    vec3##type##_t vec3##type##_scalmul(vec3##type##_t v1,           \
                                          float a);

#define DECLARE_VEC_DOT_FUNCTION(type)                               \
    float vec3##type##_dot(vec3##type##_t v1,                        \
                                vec3##type##_t v2);

#define DECLARE_VEC_NORM_FUNCTION(type)                              \
    float vec3##type##_norm(vec3##type##_t v1);

#define DECLARE_VEC_GET_UNIT_FUNCTION(type)                          \
    vec3f_t vec3##type##_unit(vec3##type##_t v1);
 

#define DEFINE_VEC_ADD_FUNCTION(type)                                   \
    vec3##type##_t vec3##type##_add(vec3##type##_t v1,                  \
                                      vec3##type##_t v2) {              \
        return (vec3##type##_t){v1.x + v2.x, v1.y + v2.y, v1.z + v2.z}; \
    }

#define DEFINE_VEC_SUB_FUNCTION(type)                                   \
    vec3##type##_t vec3##type##_sub(vec3##type##_t v1,                  \
                                    vec3##type##_t v2) {                \
        return (vec3##type##_t){v1.x - v2.x, v1.y - v2.y, v1.z - v2.z}; \
    }


#define DEFINE_VEC_SCALMUL_FUNCTION(type)                            \
    vec3##type##_t vec3##type##_scalmul(vec3##type##_t v1,           \
                                          float a) {                 \
        vec3##type##_t ret;                                          \
        ret.x = v1.x * a;                                            \
        ret.y = v1.y * a;                                            \
        ret.z = v1.z * a;                                            \
        return ret;                                                  \
    }

#define DEFINE_VEC_DOT_FUNCTION(type)                                \
    float vec3##type##_dot(vec3##type##_t v1,                        \
                                vec3##type##_t v2) {                 \
        return (float)v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;             \
    }


#define DEFINE_VEC_NORM_FUNCTION(type)                               \
    float vec3##type##_norm(vec3##type##_t v1) {                    \
        const float xx = v1.x * v1.x;                                \
        const float yy = v1.y * v1.y;                                \
        const float zz = v1.z * v1.z;                                \
        return sqrt(xx + yy + zz);                                   \
    }

#define DEFINE_VEC_GET_UNIT_FUNCTION(type)                           \
    vec3f_t vec3##type##_unit(vec3##type##_t v1) {               \
        const float n = vec3##type##_norm(v1);                       \
        return (vec3f_t) {v1.x/n, v1.y/n, v1.z/n};                   \
    }

vec3f_t vec3ito3f(vec3i32_t v);

DECLARE_VEC_ADD_FUNCTION(u8)
DECLARE_VEC_ADD_FUNCTION(i32)
DECLARE_VEC_ADD_FUNCTION(f)
DECLARE_VEC_SUB_FUNCTION(u8)
DECLARE_VEC_SUB_FUNCTION(i32)
DECLARE_VEC_SUB_FUNCTION(f)
DECLARE_VEC_SCALMUL_FUNCTION(u8)
DECLARE_VEC_SCALMUL_FUNCTION(i32)
DECLARE_VEC_SCALMUL_FUNCTION(f)
DECLARE_VEC_DOT_FUNCTION(u8)
DECLARE_VEC_DOT_FUNCTION(i32)
DECLARE_VEC_DOT_FUNCTION(f)
DECLARE_VEC_NORM_FUNCTION(u8)
DECLARE_VEC_NORM_FUNCTION(i32)
DECLARE_VEC_NORM_FUNCTION(f)
DECLARE_VEC_GET_UNIT_FUNCTION(u8)
DECLARE_VEC_GET_UNIT_FUNCTION(i32)
DECLARE_VEC_GET_UNIT_FUNCTION(f)


#endif // VMATH_H 
