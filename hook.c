#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <GL/gl.h>

typedef void (*pglLoadMatrixf_t)(const float*);
static pglLoadMatrixf_t real_pglLoadMatrixf = NULL;

void pglLoadMatrixf(const float* m) {
    if (!real_pglLoadMatrixf) {
        real_pglLoadMatrixf = (pglLoadMatrixf_t)dlsym(RTLD_NEXT, "pglLoadMatrixf");
    }
    printf("pglLoadMatrixf matrix:\n");
    for (int i = 0; i < 16; i++) printf("%f%s", m[i], (i % 4 == 3) ? "\n" : " ");
    real_pglLoadMatrixf(m);
}

__attribute__((constructor))
void init(void) {
    printf("*** OpenGL hook initialized ***\n");
    printf("*** OpenGL hook initialized ***\n");
    printf("*** OpenGL hook initialized ***\n");
}
