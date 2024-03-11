#include "window.h"

#include <cglm/cglm.h>
#include <stdlib.h>
#include <assert.h>

#define TARGET_FPS 1.0f/60.0f

struct Window window;

void window_create(func setup, func render, func update, func input) {
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window.window = glfwCreateWindow(1280, 720, "OpenGL Shooter", NULL, NULL);
    window.setup  = setup;
    window.render = render;
    window.update = update;
    window.input  = input;
    if (window.window == NULL) {
        fprintf(stderr, "Failed to create GLFW window\n");
        exit(EXIT_FAILURE);
    }

    glfwGetWindowSize(window.window, &window.width, &window.height);
    glfwMakeContextCurrent(window.window);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "Failed to initialize GLAD\n");
        exit(EXIT_FAILURE);
    }
}

void _render(void) {
    window.fps++;
    window.render();
}

void _update(void) {
    window.update();
}

void window_loop(void) {
    glViewport(0,0,window.width, window.height);

    mat4 proj = GLM_MAT4_IDENTITY_INIT;
    glm_ortho(0, window.width, 0, window.height, -1.0f, 1.0f, proj);

    window.setup();
    window.last_time = glfwGetTime();
    window.fps = 0.0f;
    float accumulator = 0.0f;
    static char buffer[42];

    while (!glfwWindowShouldClose(window.window)) {
        window.current_time = glfwGetTime();
        window.delta_time = (window.current_time - window.last_time); 
        window.last_time = window.current_time;
        
        accumulator += window.delta_time;
        if (accumulator >= 1.0f) {
            snprintf(buffer, 42, "OpenGL Shooter | FPS %.1f (%.2fms)", window.fps, 1000.0f/window.fps);
            glfwSetWindowTitle(window.window, buffer);

            window.fps = 0.0f;
            accumulator = 0.0f;
        }

        window.input();
        _update();
        _render();

        glfwPollEvents();
        glfwSwapBuffers(window.window);
    }
}

void window_destroy(void) {
    glfwDestroyWindow(window.window);
    glfwTerminate();
}
