#ifndef WINDOW_H
#define WINDOW_H
#include "gfx.h"

#define FPS 60

typedef void (*func)(void);

struct Window {
    GLFWwindow *window;
    int width, height;

    func setup, render, update, input;
    double last_time, current_time, delta_time;
    double fps;
};

void window_create(func setup, func update, func render, func input);
void window_loop(void);
void window_destroy(void);
extern struct Window window;
#endif
