#include "buffer.h"

struct Buffer buffer_create(GLenum type, bool dynamic) {
    struct Buffer buffer = {.type = type, .dynamic = dynamic};
    glGenBuffers(1, &buffer.handle);
    return buffer;
}

void buffer_bind(struct Buffer self) {
    glBindBuffer(self.type, self.handle);
}

void buffer_unbind(struct Buffer self) {
    glBindBuffer(self.type, 0);
}

void buffer_delete(struct Buffer self) {
    glDeleteBuffers(1, &self.handle);
}

void buffer_data(struct Buffer self, size_t size, void *data) {
    glBufferData(self.type, size, data, self.dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
}
