#include <stdbool.h>

#include "cglm/struct.h"
#include "gltf.h"

typedef struct CameraPerspective {
    bool has_aspect_ratio;
    float aspect_ratio;
    float yfov;
    bool has_zfar;
    float zfar;
    float znear;
} CameraPerspective;

typedef struct CameraOrthographic {
    float xmag;
    float ymag;
    float zfar;
    float znear;
} CameraOrthographic;

typedef enum CameraType {
    CameraTypePerspective,
    CameraTypeOrthographic,
} CameraType;

typedef struct Camera {
    char* name;
    CameraType type;
    union {
        CameraPerspective perspective;
        CameraOrthographic orthographic;
    } data;
} Camera;

typedef struct Node Node;
struct Node {
    char* name;
    mat4s transform;
    size_t children_len;
    Node** children;
};

typedef struct Scene {
    size_t cameras_len;
    Camera* cameras;
    size_t nodes_len;
    Node* nodes;
} Scene;

Scene scene_from_gltf_data(cgltf_data* data);

void camera_debug(Camera* camera);

void scene_debug(Scene* scene);
