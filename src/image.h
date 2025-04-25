#pragma once

#include <stddef.h>
#include <stdint.h>

#include "color.h"

struct {
    uint32_t width;
    uint32_t height;
    uint8_t* data;
} typedef Image;

Image image_new(uint32_t width, uint32_t height);

void image_free(Image image);

size_t image_size(Image image);

struct {
    uint8_t* data;
    uint32_t width;
    uint32_t height;
    size_t offset;
    size_t length;
} typedef Chunk;

Chunk image_chunk_by_index(Image image, size_t num_chunks, size_t index);

void chunk_foreach_pixel(
    Chunk chunk,
    Color (*fn)(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
);
