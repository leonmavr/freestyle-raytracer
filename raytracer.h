#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "objects.h" 
#include "types.h" 
#include "background.h" 

void rt_run(image_t* canvas, sphere_t* sph, image_t* bg);


#endif // RAYTRACER_H
