#include "obj.h"

#include <stdio.h>
#include <string.h>
#include <malloc.h>

void obj_print(obj_t *obj) {
    printf("vertices {\n");
    for (int i = 0; i < obj->vertex_count; i++) {
        printf("\tv %i: x: %f, y: %f, z: %f\n", i, obj->vertices[i].x, obj->vertices[i].y, obj->vertices[i].z);
    }
    printf("}\n");
}

obj_t *obj_load(const char *file) {
    FILE *handle = fopen(file, "r");

    if (handle == NULL) {
        return NULL;
    }

    obj_t *obj = malloc(sizeof(obj_t));

    obj->vertex_count = 0;
    obj->texcoord_count = 0;
    obj->normal_count = 0;
    obj->face_count = 0;

    // first pass, populate count's
    char line[MAX_LEN];
    while (fgets(line, MAX_LEN, handle)) {
        if (line[0] == 'o') {
            char tmp[MAX_LEN];
            if (sscanf(line, "o %255s", tmp) != 1) {
                fprintf(stderr, "Invalid obj name\n");
                fclose(handle);
                free(obj);
                return NULL;
            }
            obj->name = malloc(strlen(tmp) + 1);
            if (obj->name == NULL) {
                fprintf(stderr, "Failed to allocate obj name\n");
                fclose(handle);
                free(obj);
                return NULL;
            }
            strcpy(obj->name, tmp);
        } else if (line[0] == 'v') {
            if (line[1] == ' ') {
                obj->vertex_count++;
            } else if (line[1] == 't') {
                obj->texcoord_count++;
            } else if (line[1] == 'n') {
                obj->normal_count++;
            }
        } else if (line[0] == 'f') {
            obj->face_count += 3;
        }
    }

    obj->vertices = malloc(sizeof(vertex_t) * obj->vertex_count);
    obj->texcoords = malloc(sizeof(texcoord_t) * obj->texcoord_count);
    obj->normals = malloc(sizeof(normal_t) * obj->normal_count);
    obj->faces = malloc(sizeof(face_t) * obj->face_count);

    unsigned int vertex_index = 0;
    unsigned int texcoord_index = 0;
    unsigned int normal_index = 0;
    unsigned int face_index = 0;

    rewind(handle);
    while (fgets(line, MAX_LEN, handle)) {
        if (line[0] == 'v') {
            if (line[1] == ' ') {
                if (sscanf(line, "v %f %f %f",
                           &obj->vertices[vertex_index].x,
                           &obj->vertices[vertex_index].y,
                           &obj->vertices[vertex_index].z) != 3) {
                    fprintf(stderr, "Invalid vertex format\n");
                    fclose(handle);
                    free(obj);
                    return NULL;
                }

                vertex_index++;
            } else if (line[1] == 't') {
                if (sscanf(line, "vt %f %f",
                           &obj->texcoords[texcoord_index].u,
                           &obj->texcoords[texcoord_index].v) != 2) {
                    fprintf(stderr, "Invalid texcoord format\n");
                    fclose(handle);
                    free(obj);
                    return NULL;
                }

                texcoord_index++;
            } else if (line[1] == 'n') {
                if (sscanf(line, "vn %f %f %f",
                           &obj->normals[normal_index].x,
                           &obj->normals[normal_index].y,
                           &obj->normals[normal_index].z) != 3) {
                    fprintf(stderr, "Invalid normal format\n");
                    fclose(handle);
                    free(obj);
                    return NULL;
                }

                normal_index++;
            }
        } else if (line[0] == 'f') {
            if (sscanf(line, "f %i/%i/%i %i/%i/%i %i/%i/%i",
                       &obj->faces[face_index + 0].x,
                       &obj->faces[face_index + 0].y,
                       &obj->faces[face_index + 0].z,

                       &obj->faces[face_index + 1].x,
                       &obj->faces[face_index + 1].y,
                       &obj->faces[face_index + 1].z,

                       &obj->faces[face_index + 2].x,
                       &obj->faces[face_index + 2].y,
                       &obj->faces[face_index + 2].z) != 9) {
                fprintf(stderr, "Invalid face format\n");
                fclose(handle);
                free(obj);
                return NULL;
            }

            face_index += 3;
        }
    }

    return obj;
}

void obj_free(obj_t *obj) {
    if (obj->name != NULL)
        free(obj->name);
    if (obj->vertices != NULL)
        free(obj->vertices);
    if (obj->texcoords != NULL)
        free(obj->texcoords);
    if (obj->normals != NULL)
        free(obj->normals);
    free(obj);
}