#include "png.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

#include "color.h"

static uint32_t crc32_table[256];

void crc32_generate_table() {
    uint32_t crc;

    for (size_t i = 0; i < 256; i++) {
        crc = i;
        for (uint8_t j = 0; j < 8; j++) {
            if (crc & 1) {
                crc = (crc >> 1) ^ 0xEDB88320;
            } else {
                crc >>= 1;
            }
        }
        crc32_table[i] = crc;
    }
}

void crc32_inner(uint32_t* crc, uint8_t byte) {
    uint32_t lookup_index = (*crc ^ byte) & 0xFF;
    *crc = (*crc >> 8) ^ crc32_table[lookup_index];
}

uint32_t crc32_type_and_data(
    const uint8_t type[4], const uint8_t* ptr, size_t size
) {
    uint32_t crc = 0xFFFFFFFF;
    for (size_t i = 0; i < 4; i++) {
        crc32_inner(&crc, type[i]);
    }
    for (size_t i = 0; i < size; i++) {
        crc32_inner(&crc, ptr[i]);
    }
    return crc ^ 0xFFFFFFFF;
}

// Writes a big-endian 32-bit integer
void write_uint32_be(uint8_t* ptr, uint32_t value) {
    ptr[0] = (value >> 24) & 0xFF;
    ptr[1] = (value >> 16) & 0xFF;
    ptr[2] = (value >> 8) & 0xFF;
    ptr[3] = value & 0xFF;
}

void write_bytes(FILE* file, const uint8_t* ptr, size_t size) {
    fwrite(ptr, sizeof(uint8_t), size, file);
}

void write_uint32(FILE* file, uint32_t value) {
    uint8_t bytes[4];
    write_uint32_be(bytes, value);
    write_bytes(file, bytes, 4);
}

void write_png_header(FILE* file) {
    uint8_t header[8] = {
        0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a,
    };

    write_bytes(file, header, sizeof(header));
}

void write_chunk(
    FILE* file, const uint8_t type[4], const uint8_t* data_ptr,
    uint32_t data_size
) {
    write_uint32(file, data_size);
    write_bytes(file, type, 4);
    write_bytes(file, data_ptr, data_size);
    write_uint32(file, crc32_type_and_data(type, data_ptr, data_size));
}

// Compresses raw png image data using zlib
// Parameters:
// - raw_data: pointer to raw image data
// - raw_size: size of raw_data in bytes
// - out_size: pointer to receive size of compressed data
// Returns: malloc'd compressed buffer
uint8_t* compress_png_data(
    const uint8_t* raw_data, size_t raw_size, size_t* out_size
) {
    if (!raw_data || raw_size == 0 || !out_size) return NULL;

    // upper bound for size in bytes
    uLongf comp_bound = compressBound(raw_size);
    uint8_t* compressed = malloc(comp_bound);
    if (!compressed) return NULL;

    int res = compress2(
        compressed, &comp_bound, raw_data, raw_size, Z_BEST_COMPRESSION
    );
    if (res != Z_OK) {
        free(compressed);
        return NULL;
    }

    *out_size = comp_bound;
    return compressed;
}

uint8_t* encode_data(Image image, size_t* encoded_size) {
    size_t scanlines_size = image_size(image) + image.height;
    uint8_t* scanlines = malloc(scanlines_size);

    size_t scanline_data_size = image.width * 4;
    size_t scanline_size = scanline_data_size + 1;

    for (uint32_t y = 0; y < image.height; y++) {
        uint8_t* scanline_start = scanlines + y * scanline_size;
        scanline_start[0] = 0x00;

        uint8_t* dest = scanline_start + 1;
        uint8_t* src = image.data + y * scanline_data_size;
        memcpy(dest, src, scanline_data_size);
    }

    uint8_t* compressed_data =
        compress_png_data(scanlines, scanlines_size, encoded_size);
    if (!compressed_data) {
        printf("Compression failed.\n");
        exit(EXIT_FAILURE);
    }

    free(scanlines);
    return compressed_data;
}

void write_png(Image image) {
    FILE* file = fopen("./out.png", "wb");
    if (file == NULL) {
        printf("Error opening file ./out.png!");
        exit(1);
    }

    write_png_header(file);

    uint8_t ihdr_data[13] = {
        0x00, 0x00, 0x00, 0x00,  // width
        0x00, 0x00, 0x00, 0x00,  // height
        0x08,                    // bit depth
        0x06,                    // color type (06 = RGBA)
        0x00,                    // compression method
        0x00,                    // filter method
        0x00,                    // interlace method
    };
    write_uint32_be(ihdr_data, image.width);
    write_uint32_be(ihdr_data + 4, image.height);

    write_chunk(file, (const uint8_t*)"IHDR", ihdr_data, 13);

    size_t encoded_size;
    uint8_t* encoded_data = encode_data(image, &encoded_size);

    write_chunk(file, (const uint8_t*)"IDAT", encoded_data, encoded_size);
    free(encoded_data);

    write_chunk(file, (const uint8_t*)"IEND", NULL, 0);

    fclose(file);
}
