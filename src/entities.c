#include "entities.h"
#include "vmath.h"
#include "pixel_buffer.h"
#include "utils.h"
#include <math.h> 
#include <stdbool.h> 
#include <stdlib.h> 


#define PI 3.141592653589
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define ABS(a) ((a) > 0 ? (a) : -(a))
#define DEG2RAD(deg) ((deg) * PI / 180.0)

camera_t camera;
uint32_t** cam_pbuffer;

vec3f_t camera_project(vec3f_t xyz, bool* is_visible) {
    const float cx = camera.cx, cy = camera.cy, f = camera.f;
    // negate x and y to avoid inverted projections
    vec3f_t projected = (vec3f_t) {f*xyz.x/xyz.z - cx, f*xyz.y/xyz.z - cy, -camera.f};
    *is_visible =
        (projected.x > camera.boundary.x0 && projected.x < camera.boundary.x1) &&
        (projected.y > camera.boundary.y0 && projected.y < camera.boundary.y1);
    return projected;
}


void camera_init(float cx, float cy, float f, float fovx_deg, float fovy_deg) {
    camera.init = camera_init;
    camera.project = camera_project;
    camera.cx = cx;
    camera.cy = cy;
    camera.f = f;
    camera.boundary.x0 = MIN(f*tan(DEG2RAD(fovx_deg/2)) + cx, f*tan(DEG2RAD(-fovx_deg/2)) + cx);
    camera.boundary.x1 = MAX(f*tan(DEG2RAD(fovx_deg/2)) + cx, f*tan(DEG2RAD(-fovx_deg/2)) + cx);
    camera.boundary.y0 = MIN(f*tan(DEG2RAD(fovy_deg/2)) + cy, f*tan(DEG2RAD(-fovy_deg/2)) + cy);
    camera.boundary.y1 = MAX(f*tan(DEG2RAD(fovy_deg/2)) + cy, f*tan(DEG2RAD(-fovy_deg/2)) + cy);
}

FILE* ppm_file;
const char* ppm_filename;

void cam_pbuffer_init() {
    //// allocate pixel buffer matrix
    const int width = camera.boundary.x1 - camera.boundary.x0;
    const int height = camera.boundary.y1 - camera.boundary.y0;
    // allocate the pixel buffer - bottom 3 bytes of each element correspond to RGB
    cam_pbuffer = malloc(height * sizeof(uint32_t *));
    // TODO: check if alloc failed - pbuffer and pbuffer[0]
    for (int i = 0; i < height; i++)
        cam_pbuffer[i] = malloc(width * sizeof(cam_pbuffer[0]));
    //// open the file for writing
    ppm_filename = "output.ppm";
    ppm_file = fopen(ppm_filename, "wb");
    if (ppm_file == NULL) {
        perror("Error opening output file.\n");
        return;
    }
    fprintf(ppm_file, "P3\n%d %d\n255\n", width, height);
}

void cam_pbuffer_write(int x, int y, uint8_t r, uint8_t g, uint8_t b) {
    // as 0x00RRGGBB
    uint32_t color = (r << 16) | (g << 8) | b;
    const int width = camera.boundary.x1 - camera.boundary.x0;
    const int height = camera.boundary.y1 - camera.boundary.y0;
    // map from camera plane to 2D array indexes
    int x_idx = (int)((x - camera.boundary.x0) / (camera.boundary.x1 - camera.boundary.x0) * (width - 1));
    int y_idx = (int)((y - camera.boundary.y0) / (camera.boundary.y1 - camera.boundary.y0) * (height - 1));
    cam_pbuffer[y_idx][x_idx] = color;
}

void cam_pbuffer_save() {
    int height = camera.boundary.y1 - camera.boundary.y0;
    int width = camera.boundary.x1 - camera.boundary.x0;
    for (int r = 0; r < height; ++r) {
        for (int c = 0; c < width; ++c) {
            uint32_t color = cam_pbuffer[r][c];
            uint8_t b = color & 0xff, g = (color >> 8) & 0xff, r = (color >> 16) & 0xff;
            fprintf(ppm_file, "%u %u %u ", r, g, b);
        }
        fprintf(ppm_file, "\n");
    } 
    printf("Save ray tracing output as %s.\n", ppm_filename);
    fclose(ppm_file);
}

vec3f_t ray_at(ray_t ray, float t) {
    return (vec3f_t) {ray.origin.x + t*ray.dir.x,
                      ray.origin.y + t*ray.dir.y,
                      ray.origin.z + t*ray.dir.z};
}

ray_t ray_get(vec3f_t begin, vec3f_t end) {
    ray_t ret;
    ret.origin = begin;
    ret.dir = vec3f_unit(vec3f_sub(end,begin)); 
    return ret;
}

vec3i32_t cam2pbuffer(vec3f_t proj) {
    int px = (int)lerp(0, PBUFFER_WIDTH - 1,
                       (proj.x - camera.boundary.x0) / (camera.boundary.x1 - camera.boundary.x0));
    int py = (int)lerp(0, PBUFFER_HEIGHT - 1,
                       (proj.y - camera.boundary.y0) / (camera.boundary.y1 - camera.boundary.y0));
    return (vec3i32_t) {px, py, 0};
}


vec3u8_t hit_sphere(ray_t ray, sphere_t sphere, bool* does_intersect) {
    *does_intersect = false;
    // see https://raytracing.github.io/books/RayTracingInOneWeekend.html#addingasphere/ray-sphereintersection
    // for derivation and notation
    // ray(t) = A + tB, C = (Cx, Cy, Cz) centre of the sphere
    const float r = sphere.rad;
    vec3f_t OC = vec3f_sub(sphere.origin, ray.origin);
    float a = vec3f_dot(ray.dir, ray.dir);
    float b = -2 * vec3f_dot(ray.dir, OC);
    float c = vec3f_dot(OC, OC) - r*r;
    // the solutions of the 2nd order equation for t
    float discr = sqrt(b*b - 4*a*c);
    if (discr >= 0) {
        const float t1 = (-b + sqrt(discr))/(2*a);
        const float t2 = (-b - sqrt(discr))/(2*a);
        // keep the smallest (t0)
        const float t0 = (t1 < t2) ? t1 : t2;
        *does_intersect = true;
        // TODO: return color based on t0's location: origin + dir*t0
        return (vec3u8_t) {255, 0, 0};
    } else {
        // TODO: return background 
        return (vec3u8_t) {0, 0, 0};
    }
}
