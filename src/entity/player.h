#ifndef PLAYER_H
#define PLAYER_H
#include "../gfx/vao.h"
#include "../gfx/buffer.h"
#include "../gfx/shader.h"
#include "../gfx/texture.h"
#include "../gfx/vertex.h"
#include "collider.h"

#include <cglm/cglm.h>

struct EntityBullet {
    struct VAO vao;
    struct Buffer vbo, ibo;
    struct Vertex vertices[4];
    struct Collider collider;

    f32 x, y, w, h;
    struct EntityBullet *next, *prev;
};

struct EntityPlayer {
    struct VAO vao;
    struct Buffer vbo, ibo;
    struct Shader shader;
    struct Texture texture;
    struct Shader explosion_shader;
    struct Texture explosion_texture;
    struct Vertex vertices[4];
    struct Collider collider;
    f32 x, y, y_vel;
    ivec2 size;
    u32 score;
    
    struct Shader bullet_shader;
    struct Texture bullet_texture;
    struct EntityBullet *bullet_head;
    ivec2 bullet_size;
    u32 current_spawn_bullet;
    f32 bullet_delay;
    bool bullet_on_cooldown;
};

void player_init(void);
void player_input(void);
void player_destroy(void);
void player_render(void);
void player_update(void);
void player_shoot(void);
extern struct EntityPlayer player;
#endif
