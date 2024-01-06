#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "objects.h" 
#include "types.h" 
#include "background.h" 

void rt_run(light_t** lights, image_t* canvas, sphere_t* sph, image_t* bg, camera_t* cam);


#endif // RAYTRACER_H
