#include "image.h"

#include <stdio.h>
#include <stdlib.h>

Image image_new(uint32_t width, uint32_t height) {
    Image image = {
        .width = width,
        .height = height,
        .data = NULL,
    };
    image.data = malloc(image_size(image));
    return image;
}

void image_free(Image image) { free(image.data); }

inline size_t image_size(Image image) {
    return (image.width * 4) * image.height;
}

Chunk image_chunk_by_index(Image image, size_t num_chunks, size_t index) {
    if (num_chunks > image.width * image.height) {
        printf("Too many chunks for image!\n");
        exit(EXIT_FAILURE);
    }
    // TODO Last chunk should process the rest
    size_t length = (image.width * image.height) / num_chunks;
    Chunk chunk = {
        .data = image.data,
        .width = image.width,
        .height = image.height,
        .offset = index * length,
        .length = length,
    };

    return chunk;
}

void chunk_foreach_pixel(
    Chunk chunk,
    Color (*fn)(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
) {
    for (size_t i = 0; i < chunk.length; i++) {
        size_t pixel_index = chunk.offset + i;

        uint32_t x = pixel_index % chunk.width;
        uint32_t y = pixel_index / chunk.width;

        Color color = fn(x, y, chunk.width, chunk.height);
        uint8_t* start_pixel = chunk.data + pixel_index * 4;
        start_pixel[0] = color.r;
        start_pixel[1] = color.g;
        start_pixel[2] = color.b;
        start_pixel[3] = color.a;
    }
}
