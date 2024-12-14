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
    lights.add.ambient_light(0.3);
    lights.add.point_light(0.3, 100, 0, -600);
    lights.add.point_light(0.3, 0, 100, -600);
    lights.add.dir_light(0.3, 0.33, -0.33, 0.66);
    lights.add.dir_light(0.2, 0.1, -0.53, 0.2);
    lights.add.dir_light(0.2, 0.8, 0.53, -0.26);
    lights.normalize();

    sphere_t spheres[4];
    spheres[0] = sphere_make((vec3f_t){100, 170, 600}, 80,
                             (vec3u8_t){200, 0, 100}, 1000);
    spheres[1] = sphere_make((vec3f_t){100, 80, 1000}, 120,
                             (vec3u8_t){100, 0, 100}, 500);
    spheres[2] =
        sphere_make((vec3f_t){-50, 70, 1400}, 80, (vec3u8_t){0, 200, 50}, 300);
    spheres[3] =
        sphere_make((vec3f_t){50, -200, 600}, 80, (vec3u8_t){220, 180, 0}, 300);
    render_objects(spheres, sizeof(spheres) / sizeof(spheres[0]));

    // epilogue
    pbuffer_save("output.ppm");
    pbuffer_free();
    camera_free();
    return 0;
}
