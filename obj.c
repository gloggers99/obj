#include "obj.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

obj_t *obj_load(const char *file) {
    FILE *handle = fopen(file, "r");
    if (handle == NULL) {
        fprintf(stderr, "Failed to open file: %s\n", file);
        return NULL;
    }

    obj_t *obj = malloc(sizeof(obj_t));
    obj->numVertices = 0;
    obj->numTexCoords = 0;
    obj->numNormals = 0;
    obj->numFaces = 0;

    // First pass: count the number of each type of element
    char line[MAX_LEN];
    while (fgets(line, MAX_LEN, handle)) {
        if (line[0] == 'v') {
            if (line[1] == ' ') {
                obj->numVertices++;
            } else if (line[1] == 't') {
                obj->numTexCoords++;
            } else if (line[1] == 'n') {
                obj->numNormals++;
            }
        } else if (line[0] == 'f') {
            obj->numFaces++;
        }
    }

    // Allocate memory for the arrays
    obj->vertices = malloc(obj->numVertices * sizeof(vertex_t));
    obj->texCoords = malloc(obj->numTexCoords * sizeof(texCoord_t));
    obj->normals = malloc(obj->numNormals * sizeof(normal_t));
    obj->faces = malloc(obj->numFaces * sizeof(face_t));
    obj->fullVertices = malloc(obj->numFaces * 3 * sizeof(full_vertex_t)); // 3 vertices per face

    // Second pass: read the elements into the arrays
    fseek(handle, 0, SEEK_SET);
    size_t vertexIndex = 0;
    size_t texCoordIndex = 0;
    size_t normalIndex = 0;
    size_t faceIndex = 0;
    size_t fullVertexIndex = 0;

    while (fgets(line, MAX_LEN, handle)) {
        if (line[0] == 'v') {
            if (line[1] == ' ') {
                sscanf(line, "v %f %f %f", &obj->vertices[vertexIndex].x, &obj->vertices[vertexIndex].y, &obj->vertices[vertexIndex].z);
                vertexIndex++;
            } else if (line[1] == 't') {
                sscanf(line, "vt %f %f", &obj->texCoords[texCoordIndex].u, &obj->texCoords[texCoordIndex].v);
                texCoordIndex++;
            } else if (line[1] == 'n') {
                sscanf(line, "vn %f %f %f", &obj->normals[normalIndex].x, &obj->normals[normalIndex].y, &obj->normals[normalIndex].z);
                normalIndex++;
            }
        } else if (line[0] == 'f') {
            int v1 = -1, vt1 = -1, vn1 = -1;
            int v2 = -1, vt2 = -1, vn2 = -1;
            int v3 = -1, vt3 = -1, vn3 = -1;

            if (sscanf(line, "f %i/%i/%i %i/%i/%i %i/%i/%i", &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3) == 9 ||
                sscanf(line, "f %i//%i %i//%i %i//%i", &v1, &vn1, &v2, &vn2, &v3, &vn3) == 6 ||
                sscanf(line, "f %i/%i %i/%i %i/%i", &v1, &vt1, &v2, &vt2, &v3, &vt3) == 6) {
                obj->faces[faceIndex].v1 = v1;
                obj->faces[faceIndex].vt1 = vt1;
                obj->faces[faceIndex].vn1 = vn1;
                obj->faces[faceIndex].v2 = v2;
                obj->faces[faceIndex].vt2 = vt2;
                obj->faces[faceIndex].vn2 = vn2;
                obj->faces[faceIndex].v3 = v3;
                obj->faces[faceIndex].vt3 = vt3;
                obj->faces[faceIndex].vn3 = vn3;

                // Assemble the full vertices for the face
                for (int i = 0; i < 3; i++) {
                    int vIndex = (i == 0) ? v1 : (i == 1) ? v2 : v3;
                    int vtIndex = (i == 0) ? vt1 : (i == 1) ? vt2 : vt3;
                    int vnIndex = (i == 0) ? vn1 : (i == 1) ? vn2 : vn3;

                    if (vIndex > 0 && vIndex <= obj->numVertices) {
                        obj->fullVertices[fullVertexIndex].vertex = obj->vertices[vIndex - 1];
                    }

                    if (vtIndex > 0 && vtIndex <= obj->numTexCoords) {
                        obj->fullVertices[fullVertexIndex].texCoord = obj->texCoords[vtIndex - 1];
                        obj->fullVertices[fullVertexIndex].hasTexCoord = 1;
                    } else {
                        obj->fullVertices[fullVertexIndex].hasTexCoord = 0;
                    }

                    if (vnIndex > 0 && vnIndex <= obj->numNormals) {
                        obj->fullVertices[fullVertexIndex].normal = obj->normals[vnIndex - 1];
                        obj->fullVertices[fullVertexIndex].hasNormal = 1;
                    } else {
                        obj->fullVertices[fullVertexIndex].hasNormal = 0;
                    }

                    fullVertexIndex++;
                }

                faceIndex++;
            }
        }
    }

    fclose(handle);

    return obj;
}

void obj_free(obj_t *obj) {
    if (obj) {
        free(obj->vertices);
        free(obj->texCoords);
        free(obj->normals);
        free(obj->faces);
        free(obj->fullVertices);
        free(obj);
    }
}