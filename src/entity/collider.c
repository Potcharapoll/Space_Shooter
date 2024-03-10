#include "collider.h"

// float[4] = {x, y, w, h}
struct Collider collider_setup(struct Vertex vertices[static 4], bool render_border) {
    struct Collider collider = {
        .vertices = vertices,
        .render_border = render_border
    };
    return collider;
}

// pos[2] = {x , y}
bool collide_check(struct Collider *a, struct Collider *b) {
    return (a->vertices[0].pos[0] < b->vertices[1].pos[0] &&
            a->vertices[1].pos[0] > b->vertices[0].pos[0] &&
            a->vertices[0].pos[1] < b->vertices[2].pos[1] &&
            a->vertices[2].pos[1] > b->vertices[0].pos[1]);
}
