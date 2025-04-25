#include <stdio.h>
#include <stdlib.h>

#include "color.h"
#include "image.h"
#include "png.h"

Color shader(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
    float relative_x = (float)x / (float)width;
    float relative_y = (float)y / (float)height;

    Color color = {
        .r = (uint8_t)(relative_x * 255.0),
        .g = (uint8_t)(relative_y * 255.0),
        .b = 0x00,
        .a = 0xff,
    };

    return color;
}

int main(int argc, char* argv[]) {
    crc32_generate_table();

    printf("Args:\n");
    for (size_t i = 0; i < argc; i++) {
        printf("%s\n", argv[i]);
    }

    uint32_t width = 1920;
    uint32_t height = 1080;
    Image image = image_new(width, height);

    size_t num_chunks = 16;
#pragma omp parallel for
    for (size_t i = 0; i < num_chunks; i++) {
        Chunk chunk = image_chunk_by_index(image, num_chunks, i);
        chunk_foreach_pixel(chunk, shader);
    }

    write_png(image);
    image_free(image);
}
