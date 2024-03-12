#ifndef VERTEX_H
#define VERTEX_H
#include "../util/util.h"

struct Vertex {
    vec2 pos;
    vec2 tex;
};

void vertex_vec2_copy(vec2 * restrict src, vec2 * restrict dest);
void vertex_set_vec2_pos(struct Vertex vertex[static 4], f32 x, f32 y, f32 w, f32 h);
#endif
