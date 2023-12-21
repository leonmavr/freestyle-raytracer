#include "background.h" 
#include "types.h" 
#include <assert.h> // assert
#include <stddef.h> // size_t

void bg_create(image_t* img, vec3_u8_t* col_top, vec3_u8_t* col_bottom,
               vec3_u8_t* col_grass) {
    // draw the sky as a vertical gradient
    // TODO: clamp instead of asserting
    assert((col_top->x >= col_bottom->x) && (col_top->y >= col_bottom->y)
            && (col_top->y >= col_bottom->y));
    // how much height of the image in px (starting from the bottom) grass takes
    const unsigned height_grass_px = 0.25*HEIGHT;
    for (int i = 0; i < HEIGHT-height_grass_px; ++i) {
        // linearly interpolate from top to bottom color to paint the sky:
        // top + y/height_sky * (bottom - top)
        vec3_u8_t color = vec3_u8_sub(col_bottom, col_top);
        color = vec3_u8_scalmul(&color, -i/((float)HEIGHT-height_grass_px));
        color = vec3_u8_add(&color, col_top);
        for (int j = 0; j < WIDTH; ++j) {
            (*img)[i][j].x = color.x;
            (*img)[i][j].y = color.y;
            (*img)[i][j].z = color.z;
        }
    }

    // draw the grass as a blocky terrain of 2 aternating colors
    const size_t block_width = WIDTH/14;
    const size_t block_height = HEIGHT/14;
    vec3_u8_t col1 = *col_grass;
    vec3_u8_t col2 = vec3_u8_sub(&(vec3_u8_t){20, 30, 10}, col_grass);

    for (size_t i = HEIGHT - height_grass_px; i < HEIGHT; i += block_height) {
        for (size_t j = 0; j < WIDTH; j += block_width) {
            // alternate colors in a checkerboard pattern
            vec3_u8_t block_color = (i/block_height + j/block_width) % 2 == 0 ?
                                    col1 : col2;
            // fill the block with the chosen color
            for (size_t ii = i; ii < i + block_height && ii < HEIGHT; ++ii) {
                for (size_t jj = j; jj < j + block_width && jj < WIDTH; ++jj) {
                    (*img)[ii][jj] = block_color;
                }
            }
        }
    }
}

