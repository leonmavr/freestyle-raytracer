#include "rt2.h" 
#include "types.h" 
#include <stdbool.h>

#define DEG_TO_RAD(degrees) ((degrees) * M_PI / 180.0)

void bg_create(image_t* img, vec3_u8_t* col_top, vec3_u8_t* col_bottom) {
    // Make sure the top is lighter than the bottom so lin. int. works
    if (col_top->x < col_bottom->x) SWAP (col_top->x, col_bottom->x);
    if (col_top->y < col_bottom->y) SWAP (col_top->y, col_bottom->y);
    if (col_top->z < col_bottom->z) SWAP (col_top->z, col_bottom->z);
    // linearly interpolate from top to bottom color to paint the sky:
    // top + y/height_sky * (bottom - top)
    for (int i = 0; i < HEIGHT; ++i) {
        vec3_u8_t color = vec3_u8_sub(col_bottom, col_top);
        color = vec3_u8_scalmul(&color, -i/((float)(HEIGHT)));
        color = vec3_u8_add(&color, col_top);
        for (int j = 0; j < WIDTH; ++j) {
            (*img)[i][j].x = color.x;
            (*img)[i][j].y = color.y;
            (*img)[i][j].z = color.z;
      }
    }
}


void cam_init(cam_t* cam, i32_t cx, i32_t cy, i32_t f, float fov_deg) {
    cam->cx = cx;
    cam->cy = cy;
    cam->f = f;
    cam->fovy_rad = DEG_TO_RAD(fov_deg);
    cam->fovx_rad = (float)WIDTH/HEIGHT*DEG_TO_RAD(fov_deg);
}

void cam_move(cam_t* cam, vec3_i32_t* xyz) {
    cam->cx = xyz->x;
    cam->cy = xyz->y;
    cam->f = xyz->z;
}

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
    vec3_i32_t idiff = vec3_i32_sub(&C, &A);
    vec3_f_t fdiff = (vec3_f_t){idiff.x, idiff.y, idiff.z};
    const float b = 2*vec3_f_dot(&B, &fdiff);
    const float c = vec3_i32_dot(&idiff, &idiff) - r*r;
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


void ray_set(ray_t* ray, vec3_i32_t* start, vec3_i32_t* end) {
    ray->origin.x = start->x; 
    ray->origin.y = start->y; 
    ray->origin.z = start->z; 
    vec3_i32_t diff = vec3_i32_sub(start, end);
    vec3_f_t fdiff = (vec3_f_t) {diff.x, diff.y, diff.z};
    ray->dir = vec3_f_get_unit(&fdiff);
}


vec2_i32_t persp_transform(cam_t* cam, vec3_i32_t* pt) {
/**                                       
 *        |              |                   ^ y
 *        |              |                   |
 *        |              |                   |     __* (x, y, z)
 *        |              |                   |   _/  ^
 *        |              |                   | _/    . 
 *        |              |                   _/      .
 *        |              |                 _/|       .
 *        |              |               _/  |       .
 *        |              |            __/    |       .  y-cy
 *        |              |          _/       |       .
 *        |              |        _/         o-------.------> x
 *        |              |      _/            \      .
 *        |              |    _/               \     .
 *        |              |  _/                  \    .
 *        |              |_/                     \   .
 *       ^|. . . . . . __O (cx, cy, f). . . . . . \ .v 
 *       .|          _/  |                         \
 *       .|        _/    |                          v z
 * y'-cy .|      _/      |                           
 *       .|    _/        |              *: 3D world point             
 *       .|  _/          |              O: camera's origin             
 *       v#_/ (x', y')   |              #: projected (2D) point             
 *        |              |
 *      projection     camera
 *      plane          plane
 *      (z=0)          (z=f)
 *         
 *        From triangle similarity:
 *        (y-cy)/z = (y' - cy)/f => y' = (y-cy)/z * f + cy
 *                             ..., x' = (x-cx)/z * f + cx
 */
    i32_t x = pt->x, y = pt->y, z = pt->z;
    const i32_t cx = cam->cx, cy = cam->cy, f = cam->f;
    const i32_t ynew = round((1.0*y)*f/z) + cy;
    const i32_t xnew = round((1.0*x)*f/z)2 + cx;
    return (vec2_i32_t) {xnew, ynew};
}



void rt_run(lights_t* lights, image_t* canvas, sphere_t* sph, image_t* bg, cam_t* cam) {
    // the plane where to send the rays to
    i32_t plane_z = (float)HEIGHT/2/tan(cam->fovy_rad/2);
    // paint the canvas with the background
    for (int r = -HEIGHT/2; r < HEIGHT/2; ++r) {
        for (int c = -WIDTH/2; c < WIDTH/2; ++c) {
            const size_t ir = r + HEIGHT/2, ic = c + WIDTH/2;
            (*canvas)[ir][ic].x = (*bg)[ir][ic].x; 
            (*canvas)[ir][ic].y = (*bg)[ir][ic].y; 
            (*canvas)[ir][ic].z = (*bg)[ir][ic].z; 
        }
    }
    for (int r = -HEIGHT/2; r < HEIGHT/2; ++r) {
        for (int c = -WIDTH/2; c < WIDTH/2; ++c) {
            ray_t ray;
            ray.origin = (vec3_i32_t) {0, 0, 0};
            // where to send the ray
            vec3_i32_t end = (vec3_i32_t) {c, r, plane_z};
            ray_set(&ray, &ray.origin, &end);
            // 2D matrix indexes
            vec3_i32_t where;
            if (ray_sphere_inters(&ray, sph, &where)) {
                vec2_i32_t wherexy = persp_transform(cam, &where);
                i32_t xproj = wherexy.x + WIDTH/2;
                i32_t yproj = wherexy.y + HEIGHT/2;
                //const bool at_origin = false;
                //vec3_f_t normal = sphere_unit_normal(sph, &where, at_origin);
                float i = 1.0; //light_compute_lights(lights, &where, &normal);
                (*canvas)[yproj][xproj].x = (u8_t)(i * 255);
                (*canvas)[yproj][xproj].y = 0;
                (*canvas)[yproj][xproj].z = 0;
            }
        }
    }
}

