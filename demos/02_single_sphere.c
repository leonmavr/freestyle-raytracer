#include "camera.h"
#include "entities.h"
#include "lights.h"
#include "pixel_buffer.h"
#include "renderer.h"
#include "vmath.h"

int main()
{
    // setup
    camera_init(0, 0, 400, 60, 60);
    lights_init();
    pbuffer_init();
    lights.add.ambient_light(0.2);
    lights.add.point_light(0.3, 0, 0, -1600);
    lights.add.dir_light(0.5, 0.33, -0.33, 0.66);
    lights.add.dir_light(0.5, 0.1, -0.53, 0.26);
    lights.normalize();

    sphere_t spheres[3];
    spheres[0].origin = (vec3f_t){100, 200, 600};
    spheres[0].rad = 150;
    spheres[0].specular = 1000;
    spheres[0].color = (vec3u8_t){200, 0, 100};
    spheres[1].origin = (vec3f_t){100, 80, 1000};
    spheres[1].rad = 120;
    spheres[1].specular = 500;
    spheres[1].color = (vec3u8_t){100, 0, 200};
    spheres[2].origin = (vec3f_t){-50, 70, 800};
    spheres[2].rad = 45;
    spheres[2].specular = 400;
    spheres[2].color = (vec3u8_t){0, 200, 50};
    render_objects(spheres, 3);

    // epilogue
    pbuffer_save("output.ppm");
    pbuffer_free();
    camera_free();
    return 0;
}
