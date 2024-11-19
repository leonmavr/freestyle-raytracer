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
    int i = 0;
    //lights.light[0].type = LIGHT_AMB;
    //lights.light[0].intensity = 0.1;
    lights.light[1].type = LIGHT_POINT;
    lights.light[1].point = (vec3f_t) {1, 2, -400};
    lights.light[1].intensity = 0.9;
    //lights.light[2].type = LIGHT_DIR;
    //lights.light[2].dir = (vec3f_t) {0.33, 0.33, 0.66};
    //lights.light[2].intensity = 0.3;
    //lights.light[3].type = LIGHT_POINT;
    //lights.light[3].point = (vec3f_t) {20, 10, 10};
    //lights.light[3].intensity = 0.3;
    lights_init();
    lights.add.ambient_light(0.2);
    lights.add.point_light(0.3, 0, 0, -1600);
    lights.add.dir_light(0.5, 0.33, 0.33, 0.66);
    lights.normalize();
    sphere_t sphere;
    sphere.origin =(vec3f_t){100, 50, 800};
    sphere.rad = 150;
    //demo_camera_to_pbuffer();
    cam_pbuffer_init();
    render_objects(&sphere);
    cam_pbuffer_save("output.ppm");
    cam_pbuffer_free();
    return 0;
}
