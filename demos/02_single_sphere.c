#include "vmath.h"
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
    sphere_t sphere;
    sphere.origin =(vec3f_t){0, 0, 800};
    sphere.rad = 75;
    //demo_camera_to_pbuffer();
    cam_pbuffer_init();
    render_objects(&sphere);
    cam_pbuffer_save("output.ppm");
    cam_pbuffer_free();
    return 0;
}
