#pragma once

#include <stddef.h>
#include <stdint.h>

void crc32_generate_table();

void write_png(uint8_t* data, size_t size, uint32_t width, uint32_t height);
