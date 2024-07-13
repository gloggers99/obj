#ifndef OBJ_H
#define OBJ_H

#include <stdint.h>

#define MAX_LEN 256

typedef struct vertex {
    float x, y, z;
} vertex_t;

typedef struct texCoord {
    float u, v;
} texCoord_t;

typedef struct normal {
    float x, y, z;
} normal_t;

typedef struct face {
    int v1, vt1, vn1;
    int v2, vt2, vn2;
    int v3, vt3, vn3;
} face_t;

typedef struct full_vertex {
    vertex_t vertex;
    texCoord_t texCoord;
    normal_t normal;
    int hasTexCoord;
    int hasNormal;
} full_vertex_t;

typedef struct obj {
    vertex_t *vertices;
    texCoord_t *texCoords;
    normal_t *normals;
    face_t *faces;
    full_vertex_t *fullVertices;
    size_t numVertices;
    size_t numTexCoords;
    size_t numNormals;
    size_t numFaces;
} obj_t;

obj_t *obj_load(const char *file);
void obj_free(obj_t *obj);

#endif // OBJ_H