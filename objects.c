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
vec3_f_t sphere_unit_normal(sphere_t* sph, vec3_i32_t* where, bool at_origin) {
    vec3_i32_t normal = vec3_i32_sub(&sph->origin, where);
    const float r = sph->rad;
    vec3_f_t ret;
    vec3_f_t fwhere = (vec3_f_t) {where->x, where->y, where->z};
    ret = (vec3_f_t) {(1.0*normal.x)/r, (1.0*normal.y)/r, (1.0*normal.z)/r};
    if (!at_origin)
        ret = vec3_f_add(&ret, &fwhere);
    return ret;

}

static vec3_f_t vec3_f_unit_random() {
    // v = (a*cos(theta)*sin(phi), a*sin(theta)*sin(phi), a*cos(phi))
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
void light_add(lights_t* lights, light_type_t type, float intensity, vec3_i32_t* light_descr) {
    const size_t n = lights->n;

    if (n == 0) {
        lights->lights = (light_t**)malloc(sizeof(light_t*));
        lights->lights[0] = (light_t*)malloc(sizeof(light_t));

        // Set the first light source
        lights->lights[0]->type = type;
        lights->lights[0]->intensity = 1.0;

        if (type == LIGHT_DIR)
            lights->lights[0]->geometry.dir = *light_descr;
        else if (type == LIGHT_POINT)
            lights->lights[0]->geometry.point = *light_descr;

        lights->n = 1;
    } else {
        lights->lights = (light_t**)realloc(lights->lights, sizeof(light_t*) * (n + 1));
        lights->lights[n] = (light_t*)malloc(sizeof(light_t));

        lights->lights[n]->type = type;
        lights->lights[n]->intensity = intensity;

        if (type == LIGHT_DIR)
            lights->lights[n]->geometry.dir = *light_descr;
        else if (type == LIGHT_POINT)
            lights->lights[n]->geometry.point = *light_descr;

        // Normalize intensities
        float sum_intty = 0.0;
        for (size_t i = 0; i < n + 1; ++i)
            sum_intty += lights->lights[i]->intensity;

	if (sum_intty > 1) {
            for (size_t i = 0; i < n + 1; ++i)
                lights->lights[i]->intensity /= sum_intty;
	}

        lights->n++;
    }

}


float light_compute_lights(lights_t* lights, vec3_i32_t* point, vec3_f_t* normal) {
    float intensity = 0.0;
    const size_t n = lights->n;
    // light vector (direction/point-based)
    vec3_i32_t lvec;
    //vec3_f_t fpoint = (vec3_f_t) {point.x, point.y, point.z};
    for (size_t i = 0; i < n; ++i) {
        light_t light = *lights->lights[i];
        if (light.type == LIGHT_AMB) {
            intensity += light.intensity;
        } else {
            if (light.type == LIGHT_POINT)
                lvec = vec3_i32_sub(point, &light.geometry.point);
            else
                lvec = light.geometry.dir;
        }
        vec3_f_t flvec = (vec3_f_t) {lvec.x, lvec.y, lvec.z};
        float n_dot_l = vec3_f_dot(normal, &flvec);
        if (n_dot_l > 0) {
#if 1
            intensity += light.intensity * n_dot_l/
                (vec3_f_norm(normal) * vec3_f_norm(&flvec));
#else
/*
 *
 *                                     R
 *                         ************^              #P: ray-sphere
 *                     ****           /|****              intersection
 *                   **              /  \   **        N: unit normal at P
 *                 **               /   |     **      R: random point on  
 *                *                 |    \      *        unit sphere
 *               *                 /     |       *    PR: direction of
 *              *                 /      |        *       reflected ray
 *              *               </        \       *   r(t): ray
 *              *             N   \-- phi |       *   phi: angle between
 *               *                   \--   \     *         normal and
 *                *                     \--|    *          reflected ray
 *                 **                      |->#* P   
 *                   **               ____/ **      
 *                     ****     _____/  ****        
 *                         ____/********            
 *                    ____/                   
 *              _____^                        
 *         ____/      r(t): ray
 *      __/                                   
 *
 */
            // to follow the naming in the figure we define the following
            vec3_f_t fN = (vec3_f_t) {normal->x, normal->y, normal->z};
            vec3_f_t fR = vec3_f_unit_random();
            vec3_f_t fP = (vec3_f_t) {point->x, point->y, point->z};

            // shift normal to origin
            fN = vec3_f_sub(&fP, &fN);
            fR = vec3_f_add(&fN, &fR);
            float n_dot_r = vec3_f_dot(&fN, &fR);
            if (n_dot_r < 0) fR = vec3_f_scalmul(&fR, -1);
            // the magnitude of R is up to 2 (diameter of unit sphere)
            // so divide by 2 to normalise
            intensity += light.intensity * vec3_f_norm(&fR)/2.0;
#endif
        }
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
