#include "vmath.h"
#include "lights.h"
#include "camera.h"
#include "entities.h"
#include "pixel_buffer.h"
#include "renderer.h"
#include <stdio.h>
#include <stdlib.h>


int main() {
    // setup
    camera_init(0, 0, 400, 60, 60);
    lights_init();
    pbuffer_init();
    lights.add.ambient_light(0.2);
    lights.add.point_light(0.3, 0, 0, -1600);
    lights.add.dir_light(0.5, 0.33, -0.33, 0.66);
    lights.add.dir_light(0.5, 0.1, -0.53, 0.26);
    lights.normalize();

    sphere_t spheres[2];
    spheres[0].origin =(vec3f_t){100, 200, 1000};
    spheres[0].rad = 150;
    spheres[0].specular = 1000;
    spheres[0].color = (vec3u8_t){200, 0, 100};
    spheres[1].origin =(vec3f_t){10, 50, 600};
    spheres[1].rad = 120;
    spheres[1].specular = 500;
    spheres[1].color = (vec3u8_t){100, 0, 200};
    printf("%d\n",spheres[1].color.x);
    render_objects(spheres, 2);

    // epilogue
    pbuffer_save("output.ppm");
    pbuffer_free();
    camera_free();
    return 0;
}
