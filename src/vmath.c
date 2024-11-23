#include "vmath.h" 

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
    float vec3##type##_norm(vec3##type##_t v1) {                     \
        const float xx = v1.x * v1.x;                                \
        const float yy = v1.y * v1.y;                                \
        const float zz = v1.z * v1.z;                                \
        return sqrt(xx + yy + zz);                                   \
    }

#define DEFINE_VEC_GET_UNIT_FUNCTION(type)                           \
    vec3f_t vec3##type##_unit(vec3##type##_t v1) {                   \
        const float n = vec3##type##_norm(v1);                       \
        return (vec3f_t) {v1.x/n, v1.y/n, v1.z/n};                   \
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
DEFINE_VEC_NORM_FUNCTION(u8)
DEFINE_VEC_NORM_FUNCTION(i32)
DEFINE_VEC_NORM_FUNCTION(f)
DEFINE_VEC_GET_UNIT_FUNCTION(u8)
DEFINE_VEC_GET_UNIT_FUNCTION(i32)
DEFINE_VEC_GET_UNIT_FUNCTION(f)

vec3f_t vec3ito3f(vec3i32_t v) { return (vec3f_t) {(float)v.x, (float)v.y, (float)v.z}; }
