#include "scene.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Camera from_gltf_camera(cgltf_camera* gltf_camera) {
    Camera camera = {};

    if (gltf_camera->type == cgltf_camera_type_perspective) {
        cgltf_camera_perspective gltf_data = gltf_camera->data.perspective;

        camera.type = CameraTypePerspective;
        CameraPerspective perspective = {
            .has_aspect_ratio = gltf_data.has_aspect_ratio,
            .aspect_ratio = gltf_data.aspect_ratio,
            .yfov = gltf_data.yfov,
            .znear = gltf_data.znear,
            .has_zfar = gltf_data.has_zfar,
            .zfar = gltf_data.zfar,
        };
        camera.data.perspective = perspective;
        return camera;
    }

    if (gltf_camera->type == cgltf_camera_type_orthographic) {
        cgltf_camera_orthographic gltf_data = gltf_camera->data.orthographic;

        camera.type = CameraTypeOrthographic;
        CameraOrthographic orthographic = {
            .xmag = gltf_data.xmag,
            .ymag = gltf_data.ymag,
            .zfar = gltf_data.zfar,
            .znear = gltf_data.znear,
        };
        camera.data.orthographic = orthographic;
        return camera;
    }

    printf("Invalid camera type\n");
    exit(1);
}

mat4s compose_transform(vec3s translation, versors rotation, vec3s scale) {
    mat4s T = glms_translate_make(translation);
    mat4s R = glms_quat_mat4(rotation);
    mat4s S = glms_scale_make(scale);

    mat4s TR = glms_mat4_mul(T, R);
    mat4s M = glms_mat4_mul(TR, S);

    return M;
}

Node from_gltf_node(cgltf_node* gltf_node) {
    Node node = {
        .name = gltf_node->name,
        .children_len = gltf_node->children_count,
        .children = malloc(gltf_node->children_count * sizeof(Node*)),
    };

    // calculate transform matrix from defaults and gltf data
    if (gltf_node->has_matrix) {
        node.transform = glms_mat4_make(gltf_node->matrix);
    } else {
        vec3s translation = gltf_node->has_translation
                                ? glms_vec3_make(gltf_node->translation)
                                : GLMS_VEC3_ZERO;
        versors rotation = gltf_node->has_rotation
                               ? glms_quat_make(gltf_node->rotation)
                               : GLMS_QUAT_IDENTITY;
        vec3s scale = gltf_node->has_scale ? glms_vec3_make(gltf_node->scale)
                                           : GLMS_VEC3_ONE;
        node.transform = compose_transform(translation, rotation, scale);
    }

    for (size_t i = 0; i < node.children_len; i++) {
        /* code */
    }

    return node;
}

Scene scene_from_gltf_data(cgltf_data* data) {
    Scene scene = {
        .cameras_len = data->cameras_count,
        .cameras = malloc(data->cameras_count * sizeof(Camera)),
        .nodes_len = data->nodes_count,
        .nodes = malloc(data->nodes_count * sizeof(Node)),
    };

    for (size_t i = 0; i < scene.cameras_len; i++) {
        scene.cameras[i] = from_gltf_camera(&data->cameras[i]);
    };

    for (size_t i = 0; i < data->scene->nodes_count; i++) {
        cgltf_node* gltf_node = data->scene->nodes[i];
        scene.nodes[i] = from_gltf_node(gltf_node);
    }

    return scene;
}

void camera_debug(Camera* camera) {
    printf(
        "Camera {\n"
        "  name: \"%s\",\n",
        camera->name == NULL ? "" : camera->name
    );

    if (camera->type == CameraTypePerspective) {
        CameraPerspective data = camera->data.perspective;
        printf(
            "  has_aspect_ratio: %s,\n"
            "  aspect_ratio: %f,\n"
            "  yfov: %f,\n"
            "  has_zfar: %s,\n"
            "  zfar: %f,\n"
            "  znear: %f,\n",
            data.has_aspect_ratio ? "true" : "false", data.aspect_ratio,
            data.yfov, data.has_zfar ? "true" : "false", data.zfar, data.znear
        );
    } else {
        CameraOrthographic data = camera->data.orthographic;
        printf(
            "  xmag: %f,\n"
            "  ymag: %f,\n"
            "  zfar: %f,\n"
            "  znear: %f,\n",
            data.xmag, data.ymag, data.zfar, data.znear
        );
    }

    printf("}\n");
}

void node_debug(Node* node) {
    printf(
        "Node {\n"
        "  name: \"%s\",\n"
        "  transform: [\n",
        node->name == NULL ? "" : node->name
    );
    mat4s transform = node->transform;
    for (int row = 0; row < 4; ++row) {
        printf(
            "    [%8.4f %8.4f %8.4f %8.4f]\n", transform.raw[0][row],
            transform.raw[1][row], transform.raw[2][row], transform.raw[3][row]
        );
    }
    printf("  ],\n");

    printf("  children: [");
    for (size_t i = 0; i < node->children_len; i++) {
        if (i == 0) {
            printf("%p", node->children[i]);
        } else {
            printf(", %p", node->children[i]);
        }
    }
    printf("],\n");

    printf("}\n");
}

void scene_debug(Scene* scene) {
    // printf("Type: %u\n", data->file_type);
    // printf("Meshes: %zu\n", data->meshes_count);
    for (size_t i = 0; i < scene->cameras_len; i++) {
        camera_debug(&scene->cameras[i]);
    }

    for (size_t i = 0; i < scene->nodes_len; i++) {
        node_debug(&scene->nodes[i]);
    }
}
