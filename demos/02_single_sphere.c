#include "vmath.h"
#include "lights.h"
#include "camera.h"
#include "entities.h"
#include "pixel_buffer.h"
#include "renderer.h"
#include <stdio.h>

int main() {
    camera_init(0, 0, 400, 60, 60);
    lights_init();
    pbuffer_init();
    lights.add.ambient_light(0.2);
    lights.add.point_light(0.3, 0, 0, -1600);
    lights.add.dir_light(0.5, 0.33, -0.33, 0.66);
    lights.add.dir_light(0.5, 0.1, -0.53, 0.26);
    lights.normalize();
    sphere_t sphere;
    sphere.origin =(vec3f_t){100, 50, 800};
    sphere.rad = 150;
    sphere.specular = 1000;
    sphere.color = (vec3u8_t){200, 0, 100};
    render_objects(&sphere);
    pbuffer_save("output.ppm");
    pbuffer_free();
    return 0;
}
