#include "objects.h"
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#define DEG_TO_RAD(degrees) ((degrees) * M_PI / 180.0)


vec3_i32_t ray_at(ray_t* ray, float t) {
    vec3_f_t v = vec3_f_scalmul(&ray->dir, t);
    vec3_i32_t t_times_dir = (vec3_i32_t) {(i32_t)v.x, (i32_t)v.y, (i32_t)v.z};
    return vec3_i32_add(&ray->origin, &t_times_dir);
}

vec3_i32_t ray_sphere_inters(ray_t* ray, sphere_t* sph) {
    // see https://raytracing.github.io/books/RayTracingInOneWeekend.html#addingasphere/ray-sphereintersection
    // for derivation and notation
    // ray(t) = A + tB, C = (Cx, Cy, Cz) centre of the sphere
    vec3_i32_t A = ray->origin;
    vec3_f_t B = ray->dir;
    vec3_i32_t C = sph->origin;
    i32_t r = sph->rad;
    const float a = vec3_f_dot(&B, &B);
    // b = 2B (C - A)
    vec3_i32_t utmp = vec3_i32_sub(&C, &A);
    vec3_f_t ftmp = (vec3_f_t){utmp.x, utmp.y, utmp.z};
    const float b = 2*vec3_f_dot(&B, &ftmp);
    const float c = vec3_i32_dot(&utmp, &utmp) - r*r;
#if 0
    vec3 oc = r.origin() - center;
    auto a = dot(r.direction(), r.direction());
    auto b = 2.0 * dot(oc, r.direction());
    auto c = dot(oc, oc) - radius*radius;
    auto discriminant = b*b - 4*a*c;
#endif
    // the solutions of the 2nd order equation for t
    float discr = sqrt(b*b - 4*a*c);
    if (discr > 0) {
        const float sqrt_discr = sqrt(discr);
        const float t1 = (-b + sqrt_discr)/(2*a);
        const float t2 = (-b - sqrt_discr)/(2*a);
        // keep the smallest (t0)
        const float t0 = (t1 < t2) ? t1 : t2;
        // coordinates of intersection (A+t0*B)
        vec3_i32_t inters = (vec3_i32_t) {A.x + t0*B.x, A.y + t0*B.y,
                                        A.z + t0*B.z};
        return inters;
    } else // TODO: better way to describe intersection
        return (vec3_i32_t) {0, 0, 0};
}

/* get outward normal at a point on a sphere */
vec3_f_t sphere_unit_normal(sphere_t* sph, vec3_i32_t* where) {
    vec3_i32_t normal = vec3_i32_sub(&sph->origin, where);
    const float r = sph->rad;
    return (vec3_f_t) {(1.0*normal.x)/r, (1.0*normal.y)/r, (1.0*normal.z)/r};
}

static vec3_f_t vec3_f_unit_random() {
    // acosθsinϕ,asinθsinϕ,acosϕ)
    float phi = (float)rand() / RAND_MAX * M_PI;
    float theta = (float)rand() / RAND_MAX * M_2_PI;
    float x = cos(theta) * sin(phi);
    float y = sin(theta) * sin(phi);
    float z = cos(phi);
    return (vec3_f_t) {x, y, z};
}

vec3_u8_t sphere_reflect(sphere_t* sph, ray_t* ray) {
    vec3_i32_t inters = ray_sphere_inters(ray, sph);
    // TODO: normal from 0 to 1 not -1 to 1
    vec3_f_t normal = sphere_unit_normal(sph, &inters);
    normal = (vec3_f_t) {0.5*(normal.x + 1), 0.5*(normal.y + 1), 0.5*(normal.z + 1)};
    vec3_f_t rand_unit = vec3_f_unit_random();
    vec3_f_t ps = vec3_f_add(&normal, &rand_unit);
    if (ps.x < 0) ps.x = 0;
    if (ps.y < 0) ps.y = 0;
    if (ps.z < 0) ps.z = 0;
    vec3_f_t fcolor = (vec3_f_t) {normal.x * ps.x, 
                      normal.y * ps.y, normal.z * ps.z};
    //printf("%.2f, %.2f, %.2f\n", normal.x, normal.y, normal.z);
    //ray->origin = inters;
    //ray->dir = vec3_f_get_unit(&ps);
    float scale = 0.5*0.6*255;
    return (vec3_u8_t) {fcolor.x * scale, fcolor.y * scale, fcolor.z * scale};
}

void cam_set(camera_t* cam, i32_t cx, i32_t cy, i32_t f, float fov_deg) {
    cam->cx = cx;
    cam->cy = cy;
    cam->f = f;
    cam->fovy_rad = DEG_TO_RAD(fov_deg);
    cam->fovx_rad = (float)WIDTH/HEIGHT*DEG_TO_RAD(fov_deg);
}

bool cam_is_visible(camera_t* cam, vec3_i32_t* p) {
    // height and width of fov pyramid
    i32_t dy = HEIGHT/tan(cam->fovy_rad/2);
    i32_t dx = WIDTH/HEIGHT*dy;
    return (abs(p->x) <= dx + cam->cx) && (abs(p->y) <= dy + cam->cy);
}

void ray_set(ray_t* ray, vec3_i32_t* start, vec3_i32_t* end) {
    ray->origin.x = start->x; 
    ray->origin.y = start->y; 
    ray->origin.z = start->z; 
    vec3_i32_t diff = vec3_i32_sub(start, end);
    vec3_f_t fdiff = (vec3_f_t) {diff.x, diff.y, diff.z};
    ray->dir = vec3_f_get_unit(&fdiff);
}
