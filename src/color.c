#include "color.h"

#include <stdio.h>

Color color_from_uint32(uint32_t value) {
    Color result = {
        .r = (value >> 24) & 0xFF,
        .g = (value >> 16) & 0xFF,
        .b = (value >> 8) & 0xFF,
        .a = (value >> 0) & 0xFF,
    };
    return result;
}

void debug_color(Color color) {
    printf("Color.RGBA(%d, %d, %d, %d)\n", color.r, color.g, color.b, color.a);
}
