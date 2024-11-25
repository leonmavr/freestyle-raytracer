#include "camera.h"
#include "pixel_buffer.h"
#include <stdio.h>

void draw_square() {
    // map a small square at the center of the camera into the buffer
    // assuming the camera has already finished the projection
    int square_side = 100.0;
    for (int x = -square_side/2; x <= square_side/2; ++x) {
        for (int y = -square_side/2; y <= square_side/2; ++y) {
            cam_pbuffer_write(x, y, 255, 0, 0);
        }
    }
}


int main() {
    camera_init(50, 25, 400, 80, 80);
    draw_square();
    cam_pbuffer_save("output.ppm");
    return 0;
}
