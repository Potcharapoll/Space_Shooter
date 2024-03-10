#ifndef SHADER_H
#define SHADER_H
#include "gfx.h"

struct Shader {
    GLuint handle, vs, fs;
};

struct Shader shader_create(char *vs_path, char *fs_path);
void shader_bind(struct Shader self);
void shader_unbind(void);
void shader_delete(struct Shader self);
#endif
