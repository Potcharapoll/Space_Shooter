#ifndef COLLIDER_H
#define COLLIDER_H
#include "../gfx/vertex.h"
#include <cglm/cglm.h>
#include <stdbool.h>

struct Collider {
    struct Vertex *vertices;
    bool render_border;
};

struct Collider collider_setup(struct Vertex vertices[static 4], bool render_border);
bool collide_check(struct Collider *a, struct Collider *b);
#endif
