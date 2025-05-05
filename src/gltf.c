#include "gltf.h"

#define CGLTF_IMPLEMENTATION
#include "../include/cgltf.h"

const char* cgltf_result_to_string(cgltf_result result) {
    switch (result) {
        case cgltf_result_success:
            return "Success";
            break;
        case cgltf_result_data_too_short:
            return "Data too short";
            break;
        case cgltf_result_unknown_format:
            return "Unknown format";
            break;
        case cgltf_result_invalid_json:
            return "Invalid json";
            break;
        case cgltf_result_invalid_gltf:
            return "Invalid gltf";
            break;
        case cgltf_result_invalid_options:
            return "Invalid options";
            break;
        case cgltf_result_file_not_found:
            return "File not found";
            break;
        case cgltf_result_io_error:
            return "Io error";
            break;
        case cgltf_result_out_of_memory:
            return "Out of memory";
            break;
        case cgltf_result_legacy_gltf:
            return "Legacy gltf";
            break;
        case cgltf_result_max_enum:
            return "Max enum";
            break;
        default:
            return "Invalid result";
            break;
    }
}

cgltf_data* load_gltf_file(char* path) {
    cgltf_options options = {0};
    cgltf_data* data = NULL;

    cgltf_result result = cgltf_parse_file(&options, path, &data);
    if (result != cgltf_result_success) {
        printf("Error parsing %s\n", path);
        printf("%s\n", cgltf_result_to_string(result));
        exit(EXIT_FAILURE);
    }

    result = cgltf_load_buffers(&options, data, path);
    if (result != cgltf_result_success) {
        printf("Error loading buffers\n");
        printf("%s\n", cgltf_result_to_string(result));
        exit(EXIT_FAILURE);
    }

    result = cgltf_validate(data);
    if (result != cgltf_result_success) {
        printf("Error validating data\n");
        printf("%s\n", cgltf_result_to_string(result));
        exit(EXIT_FAILURE);
    }

    return data;
}
