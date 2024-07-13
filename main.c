#include <stdio.h>
#include <time.h>

#include "obj.h"

#define MAX_LEN 256

int main() {
    clock_t start = clock() / (CLOCKS_PER_SEC / 1000);
    const char *filename = "test.obj";
    obj_t *obj = obj_load("test.obj");
    clock_t end = clock() / (CLOCKS_PER_SEC / 1000);
    if (obj == NULL) {
        fprintf(stderr, "Failed to load OBJ file.\n");
        return 1;
    }

    printf("%s\n", obj->name);

    printf("Loaded OBJ file in %ldms\n", end - start);

    obj_free(obj);

    return 0;
}