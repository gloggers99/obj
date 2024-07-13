#ifndef OBJ_H
#define OBJ_H

#include <stdint.h>

#define MAX_LEN 256

typedef struct vec3 {
    float x, y, z;
} vec3_t;

typedef vec3_t vertex_t;

typedef struct texcoord {
    float u, v;
} texcoord_t;

typedef vec3_t normal_t;

typedef struct face {
    int x, y, z;
} face_t;

typedef struct obj {
    char *name;

    size_t vertex_count;
    size_t texcoord_count;
    size_t normal_count;
    size_t face_count;

    vertex_t *vertices;
    texcoord_t *texcoords;
    normal_t *normals;
    face_t *faces;
} obj_t;

obj_t *obj_load(const char *file);
void obj_free(obj_t *obj);

#endif // OBJ_H