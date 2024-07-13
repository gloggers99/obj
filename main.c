#include <stdio.h>
#include <time.h>

#include "obj.h"

#define MAX_LEN 256

int main() {
    clock_t start = clock() / (CLOCKS_PER_SEC / 1000);
    const char *filename = "armadillo.obj";
    obj_t *obj = obj_load(filename);
    clock_t end = clock() / (CLOCKS_PER_SEC / 1000);
    if (obj == NULL) {
        fprintf(stderr, "Failed to load OBJ file.\n");
        return 1;
    }

    printf("Loaded OBJ file in %ldms\n", end - start);

    // Add your code here to use the loaded obj
    printf("vertex X: %f\n", obj->fullVertices[0].vertex.x);
    printf("vertex Y: %f\n", obj->fullVertices[0].vertex.y);
    printf("vertex Z: %f\n", obj->fullVertices[0].vertex.z);

    return 0;
}