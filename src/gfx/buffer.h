#ifndef VBO_H
#define VBO_H
#include "gfx.h"

#include <stdbool.h>

struct Buffer {
    GLenum type;
    GLuint handle;
    bool dynamic;
};

struct Buffer buffer_create(GLenum type, bool dynamic);
void buffer_bind(struct Buffer self);
void buffer_unbind(struct Buffer self);
void buffer_delete(struct Buffer self);
void buffer_data(struct Buffer self, size_t size, void *data);
#endif
