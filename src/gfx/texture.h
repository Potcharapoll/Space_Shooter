#ifndef TEXTURE_H
#define TEXTURE_H
#include "gfx.h"
#include "../util/util.h"

struct Texture {
    s32 unit;
    GLuint handle;
};

struct Texture texture_load(s32 unit, char *path, GLenum internal_format, GLenum source_format);
void texture_bind(struct Texture texture);
void texture_unbind(void);
void texture_delete(struct Texture texture);
#endif
