#include <stdio.h>
#include <stdlib.h>

#include "color.h"
#include "png.h"

uint8_t* generate_image_data(
    size_t* size, uint32_t width, uint32_t height, Color color
) {
    *size = (1 + width * 4) * height;
    uint8_t* ptr = malloc(*size);

    for (size_t i = 0; i < height; i++) {
        uint8_t* line_start = ptr + i * (1 + width * 4);

        line_start[0] = 0x00;  // no filter
        for (size_t j = 0; j < width; j++) {
            uint8_t* color_start = line_start + 1 + j * 4;
            color_start[0] = color.r;
            color_start[1] = color.g;
            color_start[2] = color.b;
            color_start[3] = color.a;
        }
    }

    return ptr;
}

int main(int argc, char* argv[]) {
    crc32_generate_table();

    printf("Args:\n");
    for (size_t i = 0; i < argc; i++) {
        printf("%s\n", argv[i]);
    }

    Color fill = color_from_uint32(0xff00ffff);
    debug_color(fill);

    uint32_t width = 1000;
    uint32_t height = 1000;

    size_t image_len;
    uint8_t* image_ptr = generate_image_data(&image_len, width, height, fill);

    write_png(image_ptr, image_len, width, height);
    free(image_ptr);
}
