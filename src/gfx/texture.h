#ifndef TEXTURE_H
#define TEXTURE_H
#include "../gfx/gfx.h"

struct Texture {
    int unit;
    GLuint handle;
};

struct Texture texture_load(int unit, char *path, GLenum internal_format, GLenum source_format);
void texture_bind(struct Texture texture);
void texture_unbind(void);
void texture_delete(struct Texture texture);
#endif
