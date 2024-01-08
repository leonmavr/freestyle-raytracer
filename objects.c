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

bool ray_sphere_inters(ray_t* ray, sphere_t* sph, vec3_i32_t* where) {
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
    // the solutions of the 2nd order equation for t
    float discr = sqrt(b*b - 4*a*c);
    if (discr >= 0) {
        const float sqrt_discr = sqrt(discr);
        const float t1 = (-b + sqrt_discr)/(2*a);
        const float t2 = (-b - sqrt_discr)/(2*a);
        // keep the smallest (t0)
        const float t0 = (t1 < t2) ? t1 : t2;
        // coordinates of intersection (A+t0*B)
        *where = (vec3_i32_t) {A.x + t0*B.x, A.y + t0*B.y, A.z + t0*B.z};
        return true;
    } else
        return false;
}

/* get outward normal positioned at origin given a point on a sphere */
vec3_f_t sphere_unit_normal(sphere_t* sph, vec3_i32_t* where, bool normalise) {
    vec3_i32_t normal = vec3_i32_sub(&sph->origin, where);
    const float r = sph->rad;
    vec3_f_t ret;
    if (!normalise)
        ret = (vec3_f_t) {(1.0*normal.x)/r, (1.0*normal.y)/r, (1.0*normal.z)/r};
    else
        ret = (vec3_f_t) {((1.0*normal.x)/r + 1)/2, ((1.0*normal.y)/r + 1)/2, ((1.0*normal.z)/r + 1)/2};
    return ret;

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

void cam_set(camera_t* cam, i32_t cx, i32_t cy, i32_t f, float fov_deg) {
    cam->cx = cx;
    cam->cy = cy;
    cam->f = f;
    cam->fovy_rad = DEG_TO_RAD(fov_deg);
    cam->fovx_rad = (float)WIDTH/HEIGHT*DEG_TO_RAD(fov_deg);
}

/**
 * From "computer graphics from scratch" we define the following types:
 * light
 *   - type = ambient
 *   - intensity = 0.2
 * light
 *   - type = point
 *   - intensity = 0.6
 *   - position = (2, 1, 0)
 * light
 *   - type = directional
 *   - intensity = 0.2
 *   - direction = (1, 4, 4)
 */
light_t** light_add(light_t** lights, light_type_t type, float intensity, vec3_i32_t* light_descr) {
    if (lights == NULL) {
        lights = (light_t**) malloc(sizeof(light_t*));
        lights[0] = malloc(sizeof(light_t));
        // set the first light source
        lights[0]->type = type;
        lights[0]->intensity = 1.0;
        if (type == LIGHT_DIR)
            lights[0]->geometry.dir = *light_descr;
        else if (type == LIGHT_POINT)
            lights[0]->geometry.point = *light_descr;
    } else{
        const size_t n = sizeof(lights) / sizeof(lights[0]);
        lights = (light_t**) realloc(lights, sizeof(light_t*) * (n+1));
        lights[n] = (light_t*) malloc(sizeof(light_t));
        lights[n]->type = type;
        lights[n]->intensity = intensity;
        if (type == LIGHT_DIR)
            lights[n]->geometry.dir = *light_descr;
        else if (type == LIGHT_POINT)
            lights[n]->geometry.point = *light_descr;
        // normalise intensities
        float sum_intty = .0;
        for (size_t i = 0; i < n+1; ++i)
            sum_intty += lights[i]->intensity;
        for (size_t i = 0; i < n+1; ++i)
            lights[i]->intensity /= sum_intty;
    }
    return lights;
}

float light_compute_lights(light_t** lights, vec3_i32_t* point, vec3_f_t* normal) {
    float intensity = 0.0;
    const size_t n = sizeof(lights)/sizeof(lights[0]);
    printf("%d lights found\n", n);
    // light vector (direction/point-based)
    vec3_i32_t lvec;
    //vec3_f_t fpoint = (vec3_f_t) {point.x, point.y, point.z};
    for (size_t i = 0; i < n; ++i) {
        if (lights[i]->type == LIGHT_AMB) {
            intensity += lights[i]->intensity;
        } else {
            if (lights[i]->type == LIGHT_POINT)
                lvec = vec3_i32_sub(point, &lights[i]->geometry.point);
            else
                lvec = lights[i]->geometry.dir;
        }
        vec3_f_t flvec = (vec3_f_t) {lvec.x, lvec.y, lvec.z};
        // diffuse light - TODO: use Lamberdian reflection
        float n_dot_l = vec3_f_dot(normal, &flvec);
        if (n_dot_l > 0)
            intensity += lights[i]->intensity * n_dot_l/
                (vec3_f_norm(normal) * vec3_f_norm(&flvec));
    }
    return intensity;
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
