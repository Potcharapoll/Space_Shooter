#ifndef METEOR_H
#define METEOR_H
#include "../gfx/gfx.h"
#include "../gfx/shader.h"
#include "../gfx/texture.h"
#include "../gfx/vao.h"
#include "../gfx/vertex.h"
#include "../gfx/buffer.h"
#include "../util/types.h"
#include "collider.h"

struct Rock {
    struct VAO vao;
    struct Buffer ibo, vbo;
    struct Vertex vertices[4];
    struct Collider collider;
    f32 x, y, w, h;
    f32 x_vel;

    struct Rock *next, *prev;
};

struct EntityRock {
    struct Shader shader;
    struct Texture texture;
    ivec2 size;

    int current_spawn_rock;
    struct Rock *rock_head;
    bool do_spawn, on_cooldown;
    f32 spawn_delay;
};

void rock_init(void);
void rock_render(void);
void rock_update(void);
void rock_free(struct Rock **head);
void rock_destroy(void);

extern struct EntityRock rock;
#endif
