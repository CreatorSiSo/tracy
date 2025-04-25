#pragma once

#include <stdint.h>

struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
} typedef Color;

Color color_from_uint32(uint32_t value);

void debug_color(Color color);
