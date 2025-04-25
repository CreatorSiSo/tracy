#pragma once

#include <stddef.h>
#include <stdint.h>

#include "image.h"

void crc32_generate_table();

void write_png(Image image);
