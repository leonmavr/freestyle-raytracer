#include "objects.h"
#include <math.h>

vec3_u32_t ray_at(ray_t* ray, float t) {
    vec3_f_t v = vec3_f_scalmul(&ray->dir, t);
    vec3_u32_t t_times_dir = (vec3_u32_t) {(u32_t)v.x, (u32_t)v.y, (u32_t)v.z};
    return vec3_u32_add(&ray->origin, &t_times_dir);
}

vec3_u32_t ray_sphere_inters(ray_t* ray, sphere_t* sph) {
    // see https://raytracing.github.io/books/RayTracingInOneWeekend.html#addingasphere/ray-sphereintersection
    // for derivation and notation
    // ray(t) = A + tB, C = (Cx, Cy, Cz) centre of the sphere
    vec3_u32_t A = ray->origin;
    vec3_f_t B = ray->dir;
    vec3_u32_t C = sph->origin;
    u32_t r = sph->rad;
    const float a = vec3_f_dot(&B, &B);
    // b = 2B (C - A)
    vec3_u32_t utmp = vec3_u32_sub(&A, &C);
    vec3_f_t ftmp = (vec3_f_t){utmp.x, utmp.y, utmp.z};
    const float b = 2*vec3_f_dot(&B, &ftmp);
    const float c = vec3_u32_dot(&utmp, &utmp) - r*r;
    // the solutions of the 2nd order equation for t
    float sqrt_discr = sqrt(b*b - 4*a*c);
    const float t1 = (-b + sqrt_discr)/(2*a);
    const float t2 = (-b - sqrt_discr)/(2*a);
    // keep the smallest (t0)
    const float t0 = (t1 < t2) ? t1 : t2;
    // coordinates of intersection (A+t0*B)
    vec3_u32_t inters = (vec3_u32_t) {A.x + t0*B.x, A.y + t0*B.y,
                                      A.z + t0*B.z};
    return inters;
}