#pragma once

#include "../include/cgltf.h"

const char* cgltf_result_to_string(cgltf_result result);

cgltf_data* load_gltf_file(char* path);
