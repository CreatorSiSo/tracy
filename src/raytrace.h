#include <stdbool.h>

typedef struct {
    float x, y, z;
} Vec3;

typedef struct {
    float x, y, z, w;
} Vec4;

typedef struct {
    Vec3 origin;
    Vec3 direction;
} Ray;

typedef struct {
    Vec3 position;
    float radius;
    Vec4 color;
} Sphere;

typedef struct {
    bool intersected;
    float distance;
    Vec3 normal;
    Vec3 position;
} Hit;
