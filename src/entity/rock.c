#include "rock.h"
#include "player.h"
#include "../state.h"
#include "../gfx/window.h"

struct EntityRock rock;

static void _rock_new(void) {
    struct Rock *new_rock = malloc(sizeof(*new_rock));
    new_rock->vao = vao_create();
    new_rock->vbo = buffer_create(GL_ARRAY_BUFFER, true);
    new_rock->ibo = buffer_create(GL_ELEMENT_ARRAY_BUFFER, false);

    srand(glfwGetTime()*rand());
    new_rock->x = window.size[0];
    new_rock->y = rand() % (int)(window.size[1]-rock.size[1]);
    new_rock->w = rock.size[0];
    new_rock->h = rock.size[1];
    new_rock->x_vel = 2.5f;
    new_rock->next = NULL;
    new_rock->prev = NULL;
    new_rock->collider = collider_setup(new_rock->vertices, false);
    rock.current_spawn_rock++;

    vao_bind(new_rock->vao);
    buffer_bind(new_rock->vbo);
    buffer_data(new_rock->vbo, sizeof(new_rock->vertices), NULL);
    vao_attr(0, 3, GL_FLOAT, sizeof(struct Vertex), offsetof(struct Vertex, pos));
    vao_attr(1, 2, GL_FLOAT, sizeof(struct Vertex), offsetof(struct Vertex, tex));

    buffer_bind(new_rock->ibo);
    u32 indices[] = { 0,1,2,2,3,0 };
    buffer_data(new_rock->ibo, sizeof(indices), indices);

    vao_unbind();
    buffer_unbind(new_rock->vbo);
    buffer_unbind(new_rock->ibo);

    vertex_set_vec2_pos(new_rock->vertices, new_rock->x, new_rock->y, new_rock->w, new_rock->h);

    if (rock.rock_head == NULL) {
        rock.rock_head = new_rock;
        return;
    }

    struct Rock *curr = rock.rock_head;
    while (curr->next != NULL) {
        curr = curr->next;
    }
    curr->next = new_rock;
    new_rock->prev = curr;
}

void rock_init(void) {
    rock = (struct EntityRock) {
        .texture = texture_load(0, "../res/images/rock.png", GL_RGB, GL_RGB), 
        .shader = shader_create("../res/shaders/texture_rgba.vert", "../res/shaders/texture_rgba.frag"),
        .rock_head = NULL,
        .current_spawn_rock = 0,
        .spawn_delay = 0.15f,
        .do_spawn = false,
        .on_cooldown = false
    }; 
    glm_ivec2_copy((ivec2){48,46}, rock.size);

   shader_bind(rock.shader);
   shader_uniform_texture(rock.shader, rock.texture);
   shader_uniform_proj(rock.shader);
   shader_unbind();
}

void rock_render(void) {
    static double cooldown = 0.0f;

    if (state.state == INGAME) {
        rock.do_spawn = true;

        if (rock.on_cooldown) {
            cooldown += window.delta_time;

            if (cooldown >= rock.spawn_delay) {
                cooldown = 0.0f;
                rock.on_cooldown = false;
            }
        }

        if (rock.do_spawn && !rock.on_cooldown) {
            _rock_new();
            rock.on_cooldown = true;
            rock.do_spawn = false;
        }
    }

    struct Rock *head = rock.rock_head;
    texture_bind(rock.texture);
    shader_bind(rock.shader);
    while (head != NULL) {
        vao_bind(head->vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        head = head->next;
    }
    vao_unbind();
    shader_unbind();
    texture_unbind();
}

void rock_free(struct Rock **head) {
    if (*head == NULL) {
        printf("[rock_free]: Nothing to free!\n");
        return;
    }

    struct Rock *tmp = *head;
    if (*head == rock.rock_head && (*head)->next == NULL) {
        rock.rock_head = NULL;
        *head = NULL;
    }
    else if (*head == rock.rock_head && (*head)->next) {
        rock.rock_head = rock.rock_head->next;
        *head = rock.rock_head;
    }
    else if ((*head)->prev && (*head)->next) {
        (*head)->prev->next = (*head)->next;
        (*head)->next->prev = (*head)->prev;
        *head = (*head)->next;
    }
    else if ((*head)->prev && (*head)->next == NULL) {
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

    printf("free ROCK index %d\t", rock.current_spawn_rock);
    printf("remaining %d\n", --rock.current_spawn_rock);
}

void rock_update(void) {
    if (rock.current_spawn_rock <= 0) return;
    
    struct Rock *curr = rock.rock_head;
    while (curr != NULL) {
        if(collide_check(&curr->collider, &player.collider)) {
            state.state = GAMEOVER;
            player_render();
            return;
        }

        if (curr->x+rock.size[0] < 0) {
            rock_free(&curr);
            player.score++;
            continue;
        }

        curr->x-=curr->x_vel;
        vertex_set_vec2_pos(curr->vertices, curr->x, curr->y, curr->w, curr->h);

        buffer_bind(curr->vbo);
        glBufferSubData(curr->vbo.type, 0, sizeof(curr->vertices), curr->vertices);
        curr = curr->next;
    }
}

void rock_destroy(void) {
    while (rock.rock_head != NULL) {
        struct Rock *tmp = rock.rock_head;
        rock.rock_head = rock.rock_head->next;

        vao_delete(tmp->vao);
        buffer_delete(tmp->vbo);
        buffer_delete(tmp->ibo);
        free(tmp);
    }
    shader_delete(rock.shader);
    texture_delete(rock.texture);
}
