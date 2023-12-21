#include "objects.h"

vec3_u32_t ray_at(ray_t* ray, float t) {
    vec3_f_t v = vec3_f_scalmul(&ray->dir, t);
    vec3_u32_t t_times_dir = (vec3_u32_t) {(u32_t)v.x, (u32_t)v.y, (u32_t)v.z};
    return vec3_u32_add(&ray->origin, &t_times_dir);
}
