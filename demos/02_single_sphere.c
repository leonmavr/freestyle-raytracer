#include "vmath.h"
#include "lights.h"
#include "camera.h"
#include "entities.h"
#include "pixel_buffer.h"
#include "renderer.h"
#include <stdio.h>


void demo_camera_to_pbuffer() {
    // map a small square at the center of the camera into the buffer
    // assuming the camera has already finished the projection
    float square_side = 100.0;
    for (float x = -square_side / 2; x <= square_side / 2; x += 1.0) {
        for (float y = -square_side / 2; y <= square_side / 2; y += 1.0) {
            vec3f_t proj = { x, y, 0 };
            vec3i32_t pbuf_coord = cam2pbuffer(proj);
            pbuffer_write(pbuf_coord.x, pbuf_coord.y, 255, 0, 0);
        }
    }
    pbuffer_save_ppm("output.ppm");
}


int main() {
    camera_init(0, 0, 400, 60, 60);
    lights_init();
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
    cam_pbuffer_save("output.ppm");
    cam_pbuffer_free();
    return 0;
}
