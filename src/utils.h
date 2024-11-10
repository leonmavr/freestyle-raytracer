#ifndef UTILS_H
#define UTILS_H

#include <math.h>

/** Linear interpolation (lerp) between two scalars */
static inline float lerp_float(float a, float b, float t) {
    return a + t * (b - a);
}

static inline int lerp_int(int a, int b, float t) {
    return (int)(a + t * (b - a));
}

/** Linear interpolation (lerp) between two vectors */
static inline vec3u8_t lerp_vec3u8(vec3u8_t a, vec3u8_t b, float t) {
    vec3u8_t result;
    result.x = (unsigned char)(a.x + t * (b.x - a.x));
    result.y = (unsigned char)(a.y + t * (b.y - a.y));
    result.z = (unsigned char)(a.z + t * (b.z - a.z));
    return result;
}

/** Linear interpolation (lerp) between two vectors */
static inline vec3f_t lerp_vec3f(vec3f_t a, vec3f_t b, float t) {
    vec3f_t result;
    result.x = a.x + t * (b.x - a.x);
    result.y = a.y + t * (b.y - a.y);
    result.z = a.z + t * (b.z - a.z);
    return result;
}

/** Generic linear interpolation */
#define lerp(a, b, t) _Generic((a),        \
    float: lerp_float,                     \
    int: lerp_int,                         \
    vec3u8_t: lerp_vec3u8,                 \
    vec3f_t: lerp_vec3f                    \
)(a, b, t)

#endif // UTILS_H
