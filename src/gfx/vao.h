#ifndef VAO_H
#define VAO_H
#include "gfx.h"

struct VAO {
    GLuint handle;
};

struct VAO vao_create(void);
void vao_bind(struct VAO self);
void vao_unbind(void);
void vao_delete(struct VAO self);
void vao_attr(GLuint index, GLuint size, GLenum type, size_t stride, size_t offset);
#endif
