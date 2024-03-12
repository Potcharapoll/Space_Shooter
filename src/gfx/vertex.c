#include "vertex.h"

void vertex_vec2_copy(vec2 * restrict src, vec2 * restrict dest) {
    for (int i = 0; i < 2; ++i)
        *dest[i] = *src[i];
}

void vertex_set_vec2_pos(struct Vertex vertex[static 4], f32 x, f32 y, f32 w, f32 h) {
    glm_vec2_copy((vec2){x, y}, vertex[0].pos);
    glm_vec2_copy((vec2){0.0f, 0.0f}, vertex[0].tex);

    glm_vec2_copy((vec2){x+w, y}, vertex[1].pos);
    glm_vec2_copy((vec2){1.0f, 0.0f}, vertex[1].tex);

    glm_vec2_copy((vec2){x+w, y+h}, vertex[2].pos);
    glm_vec2_copy((vec2){1.0f, 1.0f}, vertex[2].tex);

    glm_vec2_copy((vec2){x, y+h}, vertex[3].pos);
    glm_vec2_copy((vec2){0.0f, 1.0f}, vertex[3].tex);
}
