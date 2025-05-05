#include <stdio.h>
#include <stdlib.h>

#include "color.h"
#include "gltf.h"
#include "image.h"
#include "png.h"

float clamp(float value) {
    return value < 0.0 ? 0.0 : (value > 1.0 ? 1.0 : value);
}

struct {
} Ray;

Color shader(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
    float coord_x = ((float)x / (float)width - 0.5) * 2.0;
    float coord_y = ((float)y / (float)height - 0.5) * 2.0;

    Color color = {
        .r = (uint8_t)(clamp(coord_x) * 255.0),
        .g = (uint8_t)(clamp(coord_y) * 255.0),
        .b = 0x00,
        .a = 0xff,
    };

    // Color color = color_from_uint32(0x000000ff);
    // size_t samples = 8;

    // for (size_t n = 0; n < samples; n += 1u) {
    //     Ray ray;
    //     ray.origin = vec3(0.0, 0.0, -2.0);
    //     ray.direction = normalize(vec3(coord_x, coord_y, 0.0) - ray.origin);

    //     Color result = trace_ray(ray);
    //     color.r += result.r;
    //     color.g += result.g;
    //     color.b += result.b;
    // }

    return color;
}

int main(int argc, char* argv[]) {
    crc32_generate_table();

    printf("Args:\n");
    for (size_t i = 0; i < argc; i++) {
        printf("%s\n", argv[i]);
    }

    cgltf_data* data = load_gltf_file(argv[1]);
    printf("Type: %u\n", data->file_type);
    printf("Meshes: %zu\n", data->meshes_count);
    cgltf_free(data);

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
