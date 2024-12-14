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
    spheres[0] = sphere_make((vec3f_t){100, 200, 600}, 150, (vec3u8_t){200, 0, 100}, 1000);
    spheres[1] = sphere_make((vec3f_t){100, 80, 1000}, 120, (vec3u8_t){100, 0, 100}, 500);
    spheres[2] = sphere_make((vec3f_t){-50, 70, 800}, 45, (vec3u8_t){0, 200, 50}, 400);
    render_objects(spheres, 3);

    // epilogue
    pbuffer_save("output.ppm");
    pbuffer_free();
    camera_free();
    return 0;
}
