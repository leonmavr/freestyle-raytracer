#include "entities.h"

#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#include "lights.h"
#include "utils.h"
#include "vmath.h"

static vec3f_t ray_at(ray_t ray, float t)
{
    return (vec3f_t){ray.origin.x + t * ray.dir.x, ray.origin.y + t * ray.dir.y,
                     ray.origin.z + t * ray.dir.z};
}

ray_t ray_get(vec3f_t begin, vec3f_t end)
{
    ray_t ret;
    ret.origin = begin;
    ret.dir = vec3f_unit(vec3f_sub(end, begin));
    return ret;
}

/* get outward normal positioned at origin given a point on a sphere */
static vec3f_t sphere_unit_normal(sphere_t sphere, vec3f_t where)
{
    vec3f_t     normal = vec3f_sub(where, sphere.origin);
    const float r = sphere.rad;
    return (vec3f_t){(1.0 * normal.x) / r, (1.0 * normal.y) / r,
                     (1.0 * normal.z) / r};
}

static ray_t shadow_ray_get(vec3f_t intersection, sphere_t sphere,
                            light_t light)
{
    const float epsilon = 0.1f;  // Small offset to avoid self-intersection
    vec3f_t     normal = vec3f_unit(vec3f_sub(intersection, sphere.origin));
    vec3f_t     offset_origin =
        vec3f_add(intersection, vec3f_scalmul(normal, epsilon));

    ray_t ret = (ray_t){.origin = offset_origin, .dir = (vec3f_t){0, 0, 0}};
    if (light.type == LIGHT_POINT)
    {
        // towards the light source
        ret.dir = vec3f_unit(vec3f_sub(light.point, offset_origin));
    }
    else if (light.type == LIGHT_DIR)
    {
        // opposite direction of the directional light
        ret.dir = vec3f_scalmul(light.dir, -1.0f);
    }
    return ret;
}

static bool ray_hits_sphere(ray_t ray, sphere_t sphere)
{
    // see
    // https://raytracing.github.io/books/RayTracingInOneWeekend.html#addingasphere/ray-sphereintersection
    // for derivation and notation
    // ray(t) = A + tB, C = (Cx, Cy, Cz) centre of the sphere
    const float r = sphere.rad;
    vec3f_t     OC = vec3f_sub(sphere.origin, ray.origin);
    float       a = vec3f_dot(ray.dir, ray.dir);
    float       b = -2 * vec3f_dot(ray.dir, OC);
    float       c = vec3f_dot(OC, OC) - r * r;
    // the solutions of the 2nd order equation for t
    float discr = sqrt(b * b - 4 * a * c);
    return discr >= 0;
}

static vec3f_t vec3f_unit_random()
{
    // v = (a*cos(theta)*sin(phi), a*sin(theta)*sin(phi), a*cos(phi))
    float phi = (float)rand() / RAND_MAX * UT_PI;
    float theta = (float)rand() / RAND_MAX * UT_TWO_PI;
    float x = cos(theta) * sin(phi);
    float y = sin(theta) * sin(phi);
    float z = cos(phi);
    return (vec3f_t){x, y, z};
}

// compute the effect of all lights at a point on an object
float lights_on_sphere(sphere_t sphere, vec3f_t inters, vec3f_t unit_norm,
                       vec3f_t raydir, float specular, sphere_t* spheres,
                       int num_spheres)
{
    float intensity = 0.0;
    for (int i = 0; i < LIGHTS_CAPACITY; ++i)
    {
        if (lights.light[i].type == LIGHT_AMB)
        {
            intensity += lights.light[i].intensity;
        }
        else
        {
            // light's direction vector
            vec3f_t L = {0, 0, 0};
            // find the direction from the source of light
            if (lights.light[i].type == LIGHT_POINT)
                L = vec3f_sub(lights.light[i].point, inters);
            else if (lights.light[i].type == LIGHT_DIR)
                L = lights.light[i].dir;
            ray_t shadow_ray = shadow_ray_get(inters, sphere, lights.light[i]);
            bool  is_shadow = false;
            for (int j = 0; j < num_spheres; ++j)
            {
                // TODO: self-intersection
                bool self_inters = (spheres[j].rad < (sphere.rad + 0.1)) &&
                                   (spheres[j].rad > (sphere.rad - 0.1));
                if (!self_inters && ray_hits_sphere(shadow_ray, spheres[j]))
                    is_shadow = true;
            }
            if (is_shadow) continue;

            // diffuse reflection
            if (vec3f_dot(L, unit_norm) > 0)
                intensity += lights.light[i].intensity *
                             vec3f_dot(L, unit_norm) / (vec3f_norm(L));
            // specular reflection
            if (specular > -0.99 || specular < -1.01)
            {
                vec3f_t R = vec3f_sub(
                    vec3f_scalmul(
                        vec3f_scalmul(unit_norm, vec3f_dot(unit_norm, L)), 2.0),
                    L);

                float raydir_dot_R = vec3f_dot(raydir, R);
                if (raydir_dot_R > 0)
                {
                    // ray dir needs to be a unit vector
                    intensity += lights.light[i].intensity *
                                 pow(raydir_dot_R / (vec3f_norm(R)), specular);
                }
            }
        }
    }
    return intensity;
}

vec3u8_t hit_sphere(ray_t ray, sphere_t sphere, bool* does_intersect,
                    float* dist, sphere_t* spheres, int num_spheres)
{
    *does_intersect = false;
    // see
    // https://raytracing.github.io/books/RayTracingInOneWeekend.html#addingasphere/ray-sphereintersection
    // for derivation and notation
    // ray(t) = A + tB, C = (Cx, Cy, Cz) centre of the sphere
    const float r = sphere.rad;
    vec3f_t     OC = vec3f_sub(sphere.origin, ray.origin);
    float       a = vec3f_dot(ray.dir, ray.dir);
    float       b = -2 * vec3f_dot(ray.dir, OC);
    float       c = vec3f_dot(OC, OC) - r * r;
    // the solutions of the 2nd order equation for t
    float discr = sqrt(b * b - 4 * a * c);
    if (discr >= 0)
    {
        const float t1 = (-b + sqrt(discr)) / (2 * a);
        const float t2 = (-b - sqrt(discr)) / (2 * a);
        // keep the smallest (t0)
        const float t0 = (t1 < t2) ? t1 : t2;
        *does_intersect = true;
        vec3f_t inters = ray_at(ray, t0);
        vec3f_t normal_unit = sphere_unit_normal(sphere, inters);
        float   intty = lights_on_sphere(sphere, inters, normal_unit, ray.dir,
                                         sphere.specular, spheres, num_spheres);
        *dist = vec3f_norm(vec3f_sub(ray.origin, inters));
        return (vec3u8_t){intty * sphere.color.x, intty * sphere.color.y,
                          intty * sphere.color.z};
    }
    else
    {
        // TODO: return background
        return (vec3u8_t){0, 0, 0};
    }
}
