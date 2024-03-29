#include "player.h"
#include "rock.h"
#include "collider.h"
#include "../state.h"
#include "../gfx/window.h"
#include "../gfx/vertex.h"

struct EntityPlayer player;

static void _bullet_destroy(struct EntityBullet **head) {
    if (*head == NULL) {
        printf("[Bullet_free]: Nothing to free\n");
        return;
    }

    struct EntityBullet *tmp = *head;
    if (*head == player.bullet_head && (*head)->next) {
        player.bullet_head = player.bullet_head->next;
        *head = player.bullet_head; }
    else if (*head == player.bullet_head && (*head)->next == NULL) {
        player.bullet_head = NULL;
        *head = NULL;
    }
    else if ((*head)->prev && (*head)->next) {
        (*head)->prev->next = (*head)->next;
        (*head)->next->prev = (*head)->prev;
        *head = (*head)->next;
    }
    else if ((*head)->next == NULL && (*head)->prev) {
        (*head)->prev->next = NULL;
        *head = NULL;
    }
    else {
        *head = (*head)->next;
        (*head)->prev = NULL;
    }

    vao_delete(tmp->vao);
    buffer_delete(tmp->vbo);
    buffer_delete(tmp->ibo);
    free(tmp);

    printf("free BULLET index %d", player.current_spawn_bullet--);
    printf("\tremaining %d\n", player.current_spawn_bullet);
}

static void bullet_update(struct EntityBullet *head) {
    if (player.current_spawn_bullet <= 0) return;
    
    while (head != NULL) {
        struct Rock *m = rock.rock_head;
        while (m != NULL && head != NULL) {
            if (collide_check(&m->collider, &head->collider)) {
                player.score++;
                rock_free(&m);
                _bullet_destroy(&head);
                continue;
            }
            m = m->next;
        }

        if (head == NULL) break;

        if (head->x > window.size[0]) {
            _bullet_destroy(&head);
            continue;
        }

        head->x += 5.0f;
        vertex_set_vec2_pos(&head->vertices[0], head->x, head->y, head->w, head->h);

        buffer_bind(head->vbo);
        glBufferSubData(head->vbo.type, 0, sizeof(head->vertices), head->vertices);

        head = head->next;
    }
}

static void bullet_new(f32 x, f32 y, f32 w, f32 h) {
    struct EntityBullet *new_bullet = malloc(sizeof(*new_bullet));
    new_bullet->vao = vao_create();
    new_bullet->vbo = buffer_create(GL_ARRAY_BUFFER, true);
    new_bullet->ibo = buffer_create(GL_ELEMENT_ARRAY_BUFFER, false);
    new_bullet->collider = collider_setup(new_bullet->vertices, false);
    new_bullet->x = x;
    new_bullet->y = y;
    new_bullet->w = w;
    new_bullet->h = h;
    new_bullet->next = NULL;
    new_bullet->prev = NULL;
    player.current_spawn_bullet++;
    vertex_set_vec2_pos(&new_bullet->vertices[0], x, y, w, h);

    vao_bind(new_bullet->vao);
    buffer_bind(new_bullet->vbo);
    buffer_data(new_bullet->vbo, sizeof(new_bullet->vertices), NULL);
    vao_attr(0, 2, GL_FLOAT, sizeof(struct Vertex), offsetof(struct Vertex, pos));
    vao_attr(1, 2, GL_FLOAT, sizeof(struct Vertex), offsetof(struct Vertex, tex));

    buffer_bind(new_bullet->ibo);
    u32 indices[] = {0,1,2,2,3,0};
    buffer_data(new_bullet->ibo, sizeof(indices), indices);

    vao_unbind();
    buffer_unbind(new_bullet->vbo);
    buffer_unbind(new_bullet->ibo);

    if (player.bullet_head == NULL) {
        player.bullet_head = new_bullet;
        return;
    }

    struct EntityBullet *curr = player.bullet_head;
    while (curr->next != NULL) 
        curr = curr->next;
    
    new_bullet->prev = curr;
    curr->next = new_bullet;
}

static void bullet_render(struct EntityBullet *head) {
    if (player.current_spawn_bullet <= 0) return;

    texture_bind(player.bullet_texture);
    shader_bind(player.bullet_shader);
    while (head != NULL) {
        vao_bind(head->vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        head = head->next;
    }
    vao_unbind();
    shader_unbind();
    texture_unbind();
}

void player_init(void) {
   player = (struct EntityPlayer) {
        .x = 20.0f,
        .y = 300.0f,
        .y_vel = 0.0f,
        .texture = texture_load(0, "../res/images/player.png", GL_RED, GL_RG),
        .shader = shader_create("../res/shaders/texture.vert", "../res/shaders/texture.frag"),
        .collider = collider_setup(player.vertices, false),
        .explosion_texture = texture_load(0, "../res/images/explosion.png", GL_RGBA, GL_RGBA),
        .explosion_shader = shader_create("../res/shaders/texture_rgba.vert", "../res/shaders/texture_rgba.frag"), 
        .vao = vao_create(),
        .ibo = buffer_create(GL_ELEMENT_ARRAY_BUFFER, false),
        .vbo = buffer_create(GL_ARRAY_BUFFER, true),
        .score = 0,

        .bullet_texture = texture_load(1, "../res/images/bullet.png", GL_RED, GL_RGB),
        .bullet_shader = shader_create("../res/shaders/texture.vert", "../res/shaders/texture.frag"),
        .bullet_head = NULL,
        .bullet_on_cooldown = false,
        .bullet_delay = 0.55f,
   };
   glm_ivec2_copy((ivec2){48,46}, player.size);
   glm_ivec2_copy((ivec2){26,9}, player.bullet_size);

   shader_bind(player.shader); 
   shader_uniform_texture(player.shader, player.texture);
   shader_uniform_proj(player.shader);
   shader_unbind();
   
   shader_bind(player.explosion_shader); 
   shader_uniform_texture(player.explosion_shader, player.explosion_texture);
   shader_uniform_proj(player.explosion_shader);
   shader_unbind();

   shader_bind(player.bullet_shader);
   shader_uniform_texture(player.bullet_shader, player.bullet_texture);
   shader_uniform_proj(player.bullet_shader);
   shader_unbind();

   vao_bind(player.vao);

   buffer_bind(player.vbo);
   buffer_data(player.vbo, sizeof(player.vertices), NULL);
   vao_attr(0, 4, GL_FLOAT, 4*sizeof(float), 0);

   buffer_bind(player.ibo);
   u32 indices[] = { 0, 1, 2, 2, 3, 0 };
   buffer_data(player.ibo, sizeof(indices), indices);

   vao_unbind();
   buffer_unbind(player.vbo);
   buffer_unbind(player.ibo);
}

void player_input(void) {
    if (state.state == INGAME) {
        if (glfwGetKey(window.window, GLFW_KEY_W) == GLFW_PRESS ) {
            player.y += 2.0f; 
        }
        if (glfwGetKey(window.window, GLFW_KEY_S) == GLFW_PRESS ) {
            player.y += -2.0f;
        }

        if (glfwGetKey(window.window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) {
            player_shoot();
        }
    }
}

void player_destroy(void) {
    texture_delete(player.bullet_texture);
    shader_delete(player.bullet_shader);

    if (player.bullet_head) {
        while (player.bullet_head != NULL) {
            struct EntityBullet *curr = player.bullet_head;
            player.bullet_head = player.bullet_head->next;
            free(curr);
        }
    }

    texture_delete(player.explosion_texture);
    shader_delete(player.explosion_shader);

    texture_delete(player.texture);
    shader_delete(player.shader);
    buffer_delete(player.vbo);
    buffer_delete(player.ibo);
    vao_delete(player.vao);
}

void player_render(void) {
    if (state.state != GAMEOVER) {
        shader_bind(player.shader);
        texture_bind(player.texture);
    }
    else {
        shader_bind(player.explosion_shader);
        texture_bind(player.explosion_texture);
    }

    vao_bind(player.vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    bullet_render(player.bullet_head);
}

void player_update(void) {  

    if (player.y < 0) {
        player.y = 0;
    }

    if (player.y+player.size[1] > window.size[1]) {
        player.y = window.size[1] - player.size[1];
    }

    vertex_set_vec2_pos(player.vertices, player.x, player.y, player.size[0], player.size[1]);

    buffer_bind(player.vbo);
    glBufferSubData(player.vbo.type, 0, sizeof(player.vertices), player.vertices);

    bullet_update(player.bullet_head);
}

void player_shoot(void) {
    static double cooldown = 0.0f;
    if (player.bullet_on_cooldown) {
        cooldown += window.delta_time;
        if (cooldown >= player.bullet_delay) {
            cooldown = 0.0f;
            player.bullet_on_cooldown = false;
        }
    }
    
    if (!player.bullet_on_cooldown) {
        bullet_new(player.x+player.size[0]+5.0f, player.y+player.size[1]/2.0-5.0f, player.bullet_size[0], player.bullet_size[1]); 
        player.bullet_on_cooldown = true;
    }

}
