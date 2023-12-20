#include "background.h" 
#include "types.h" 

image_t bg_create(vec3_u8_t* color_top, vec3_u8_t* color_bottom,
                  vec3_u8_t* color_grass) {
    image_t ret;
    // how much height of the image in px (starting from the bottom) grass takes
    const unsigned height_grass_px = 0.25*HEIGHT;
    // linearly interpolate from top to bottom color to paint the sky
    for (int i = 0; i < (HEIGHT-height_grass_px); ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            
        }
    }
    // draw the grass as a blocky terrain of 2 colors

    return ret;
}

