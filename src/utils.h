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

static inline float lmap_float(float x, float a, float b, float c, float d) {
    float t = (x - a) / (b - a);
    return lerp_float(c, d, t);
}

static inline int lmap_int(int x, int a, int b, int c, int d) {
    float t = (float)(x - a) / (b - a);
    return lerp_int(c, d, t);
}

static inline vec3u8_t lmap_vec3u8(vec3u8_t x, vec3u8_t a, vec3u8_t b, vec3u8_t c, vec3u8_t d) {
    vec3u8_t t;
    t.x = (float)(x.x - a.x) / (b.x - a.x);
    t.y = (float)(x.y - a.y) / (b.y - a.y);
    t.z = (float)(x.z - a.z) / (b.z - a.z);
    return lerp_vec3u8(c, d, t.x);
}


static inline vec3f_t lmap_vec3f(vec3f_t x, vec3f_t a, vec3f_t b, vec3f_t c, vec3f_t d) {
    vec3f_t t;
    t.x = (x.x - a.x) / (b.x - a.x);
    t.y = (x.y - a.y) / (b.y - a.y);
    t.z = (x.z - a.z) / (b.z - a.z);
    return lerp_vec3f(c, d, t.x);
}

/** Generic linear interpolation */
#define lerp(a, b, t) _Generic((a),                         \
    float: lerp_float,                                      \
    int: lerp_int,                                          \
    vec3u8_t: lerp_vec3u8,                                  \
    vec3f_t: lerp_vec3f                                     \
)(a, b, t)


/** Generic linear map from range [a, b] to [c, d] */
#define lmap(x, a, b, c, d) _Generic((x),                  \
    float: lmap_float,                                     \
    int: lmap_int,                                         \
    vec3u8_t: lmap_vec3u8,                                 \
    vec3f_t: lmap_vec3f                                    \
)(x, a, b, c, d)

#endif // UTILS_H
