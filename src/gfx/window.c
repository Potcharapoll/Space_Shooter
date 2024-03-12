#include "window.h"

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

    glfwGetWindowSize(window.window, &window.size[0], &window.size[1]);
    glfwMakeContextCurrent(window.window);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "Failed to initialize GLAD\n");
        exit(EXIT_FAILURE);
    }
}

static inline void _render(void) {
    window.render();
}

static inline void _update(void) {
    window.update();
}

static inline void _process_input(void) {
    window.input();
}

void window_loop(void) {
    glViewport(0,0,window.size[0], window.size[1]);

    mat4 proj = GLM_MAT4_IDENTITY_INIT;
    glm_ortho(0, window.size[0], 0, window.size[1], -1.0f, 1.0f, proj);

    window.setup();
    f32 last_time = glfwGetTime();
    f32 current_time;
    f32 total_time = 0.0f;
    char buffer[42];

    while (!glfwWindowShouldClose(window.window)) {
        current_time = glfwGetTime();
        window.delta_time = (current_time - last_time); 
        last_time = current_time;

        _process_input();
        _update();
        _render();

        glfwSwapBuffers(window.window);
        glfwPollEvents();

        total_time += window.delta_time;

        if (total_time >= 1.0) {
            snprintf(buffer, sizeof(buffer), "OpenGL Shooter | FPS %.1f (%.2fms)", 
                    1.0f/window.delta_time, window.delta_time*1000.0f);

            glfwSetWindowTitle(window.window, buffer);
            total_time = 0.0f;
        } 
    }
}

void window_destroy(void) {
    glfwDestroyWindow(window.window);
    glfwTerminate();
}
