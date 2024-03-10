#include "state.h"
#include "gfx/window.h"
#include "gfx/text.h"
#include "entity/player.h"
#include "entity/rock.h"

struct State state;
struct Text txt;

static char buffer[108];

// font.png 64x40
// TODO:
//  - Fix Collition and Entity Rendering (100%) [Everything works fine now 02:25 (02/09/67)]
//  - Render Bitmap Font [Everything work file now 04:16 (02/10/67)]
//  - Refactor
//  - Menu (START, PAUSE) [PASSED]
//  - Score counter       [PASSED]
//  - Game over           [PASSED]
//  - Restart or Quit     [PASSED]

void setup(void) {
    state.ingame = false;
    state.state = MENU;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glm_ortho(0, window.width, 0, window.height, 0.0f, 1.0f, state.proj);
    txt = text_load("../res/textures/font.png", 64.0f, 40.0f, 8.0f);
}

static inline void ingame_setup(void) {
    player_init();
    rock_init();
}

void update(void) {
    if (state.ingame) {
        player_update();
        rock_update();
    } 
}

void render(void) {
    if (state.state == MENU) {
        glClearColor(0.2f, 0.2f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        text_render(txt, "SPACE SHOOTER", (window.width - (8.0*10.0*13.0))/2, window.height - 250.0f, 10.0f, (vec3){1.0f, 0.5f, 1.0f});
        text_render(txt, "PRESS SPACE TO START", (window.width - (8.0*3.0*20.0))/2, window.height - 500.0f, 3.0f, (vec3){1.0f, 1.0f, 1.0f});
    }

    if (state.state == GAMEOVER) {
        text_render(txt, "GAME OVER", (window.width - (8.0*8.0*9.0))/2, window.height - 350.0f, 8.0f, (vec3){1.0f, 0.0f, 0.0f});
        text_render(txt, "PRESS SPACE TO RESTART OR ESCAPE TO EXIT", (window.width - (8.0*3.0*40.0))/2, window.height - 450.0f, 3.0f, (vec3){1.0f, 1.0f, 1.0f});
    }

    if (state.ingame || state.state == PAUSE) {
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        player_render();
        rock_render();
        snprintf(buffer, 108, "SCORE %u", player.score);
        text_render(txt, buffer, 20.0f, window.height - 60.0f, 5.0f, (vec3){1.0f, 1.0f, 1.0f});

        if (state.state == PAUSE) {
            text_render(txt, "PAUSE THE GAME", (window.width - (8.0*8.0*14.0))/2, window.height - 350.0f, 8.0f, (vec3){0.5f, 0.7f, 0.2f});
            text_render(txt, "PRESS ESCAPE TO CONTINUE", (window.width - (8.0*3.0*24.0))/2, window.height - 450.0f, 3.0f, (vec3){1.0f, 1.0f, 1.0f});
        }

    }

}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (state.state == MENU || state.state == GAMEOVER) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            exit(0);
        }

        if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
            state.state = INGAME;
            state.ingame = true;
            ingame_setup();
        }
    }

    if ((state.state == INGAME && state.ingame) || (state.state == PAUSE)) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            if (state.ingame) {
                state.ingame = false;
                state.state = PAUSE;
            }
            else {
                state.ingame = true;
                state.state = INGAME;
            }
        }
    }
 }

void input(void) {
    glfwSetKeyCallback(window.window, key_callback);

    if (state.ingame && state.state == INGAME) {
        player_input();
        return;
    }
}

void cleanup(void) {
    player_destroy();
    rock_destroy();
    text_destroy(txt);
    window_destroy();
}

int main(void) {
    atexit(cleanup);
    window_create(setup, render, update, input);
    window_loop();
    return 0;
}
