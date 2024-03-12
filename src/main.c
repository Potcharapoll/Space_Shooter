#include "state.h"
#include "util/util.h"
#include "gfx/window.h"
#include "gfx/text.h"
#include "entity/player.h"
#include "entity/rock.h"

struct State state;

static struct Text txt;
static f32 timer1 = 0.0f;
static char buffer[108];

// remove later
struct Obj {
    struct VAO vao;
    struct Buffer vbo, ibo;
    struct Texture texture;
    struct Shader shader;
} bg;

// font.png 64x40
// TODO:
//  - Fix Collition and Entity Rendering (100%) [Everything works fine now 02:25 (02/09/67)]
//  - Render Bitmap Font  [Everything work file now 04:16 (02/10/67)]
//  - Menu (START, PAUSE) [PASSED]
//  - Score counter       [PASSED]
//  - Game over           [PASSED]
//  - Restart or Quit     [PASSED]
//
//  - Refactor            [1]
//  - Sound               [Can't be true for now (02/10/67)]

void setup(void) {
    state.state = MENU;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glm_ortho(0, window.size[0], 0, window.size[1], 0.0f, 1.0f, state.proj);
    txt = text_load("../res/images/font.png", (vec2){64.0f, 40.0f}, 8.0f);

    bg = (struct Obj) {
        .vao = vao_create(),
        .vbo = buffer_create(GL_ARRAY_BUFFER, false),
        .ibo = buffer_create(GL_ELEMENT_ARRAY_BUFFER, false),
        .shader = shader_create("../res/shaders/texture_rgba.vert", "../res/shaders/texture_rgba.frag"),
        .texture = texture_load(0, "../res/images/background.png", GL_RGBA, GL_RGBA)
    };

    float vertices[] = {
        0.0,            0.0,               0.0, 0.0,
        window.size[0], 0.0,               1.0, 0.0,
        window.size[0], window.size[1],    1.0, 1.0,   
        0.0,            window.size[1],    0.0, 1.0
    };

    u32 indices[] = {0,1,2,2,3,0};

    vao_bind(bg.vao);
    buffer_bind(bg.vbo);
    buffer_data(bg.vbo, sizeof(vertices), vertices);
    vao_attr(0, 4, GL_FLOAT, 4*sizeof(f32), 0);

    buffer_bind(bg.ibo);
    buffer_data(bg.ibo, sizeof(indices), indices);

    shader_bind(bg.shader);
    shader_uniform_texture(bg.shader, bg.texture);
    shader_uniform_proj(bg.shader);
    shader_unbind();
    vao_unbind();
    buffer_unbind(bg.vbo);
    buffer_unbind(bg.ibo);
}

static inline void ingame_setup(void) {
    player_init();
    rock_init();
}

void update(void) {
    if (state.state == INGAME) {
        player_update();
        rock_update();
    } 
}

void render(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.2f, 0.2f, 0.4f, 1.0f);

    texture_bind(bg.texture);
    shader_bind(bg.shader);
    vao_bind(bg.vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    if (state.state == MENU) {

        text_render(txt, "SPACE SHOOTER", (vec2){(window.size[0] - (8.0*10.0*13.0))/2, window.size[1] - 250.0f},
                10.0f, (vec3){0.4f, 0.4f, 1.0f});

        text_blink_render(txt, "PRESS SPACE TO START", (vec2){(window.size[0] - (8.0*3.0*20.0))/2, window.size[1] - 500.0f},
                3.0f, (vec3){1.0f, 1.0f, 1.0f}, 0.75f, &timer1);
    }

    if (state.state == GAMEOVER) {

        text_render(txt, "GAME OVER", (vec2){(window.size[0] - (8.0*8.0*9.0))/2, window.size[1] - 350.0f},
                8.0f, (vec3){1.0f, 0.0f, 0.0f});

        text_render(txt, "PRESS SPACE TO RESTART OR ESCAPE TO EXIT", (vec2){(window.size[0] - (8.0*3.0*40.0))/2, window.size[1] - 450.0f},
                3.0f, (vec3){1.0f, 1.0f, 1.0f});
    }

    if (state.state == PAUSE) {

        text_render(txt, "PAUSE THE GAME", (vec2){(window.size[0] - (8.0*8.0*14.0))/2, window.size[1] - 350.0f},
                8.0f, (vec3){0.5f, 0.7f, 0.2f});

        text_render(txt, "PRESS ESCAPE TO CONTINUE", (vec2){(window.size[0] - (8.0*3.0*24.0))/2, window.size[1] - 450.0f},
                3.0f, (vec3){1.0f, 1.0f, 1.0f});
    }

    if (state.state == INGAME) {
        player_render();
        rock_render();
        snprintf(buffer, sizeof(buffer), "SCORE %u", player.score);
        text_render(txt, buffer, (vec2){20.0f, window.size[1] - 60.0f}, 5.0f, (vec3){1.0f, 1.0f, 1.0f});

    }
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (state.state == MENU || state.state == GAMEOVER) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            exit(0);
        }

        if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
            state.state = INGAME;
            ingame_setup();
        }
    }

    if ((state.state == INGAME) || (state.state == PAUSE)) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            if (state.state == INGAME) {
                state.state = PAUSE;
            }
            else {
                state.state = INGAME;
            }
        }
    }
}

void input(void) {
    glfwSetKeyCallback(window.window, key_callback);

    if (state.state == INGAME) {
        player_input();
        return;
    }
}

void cleanup(void) {
    player_destroy();
    rock_destroy();
    text_destroy(txt);

    vao_delete(bg.vao);
    buffer_delete(bg.vbo);
    buffer_delete(bg.ibo);
    texture_delete(bg.texture);
    shader_delete(bg.shader);

    window_destroy();
}

int main(void) {
    atexit(cleanup);
    window_create(setup, render, update, input);
    window_loop();
    return 0;
}
