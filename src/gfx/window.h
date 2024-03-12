#ifndef WINDOW_H
#define WINDOW_H
#include "gfx.h"
#include "../util/util.h"

typedef void (*func)(void);

struct Window {
    GLFWwindow *window;
    ivec2 size;

    func setup, render, update, input;
    f32 delta_time;
};

void window_create(func setup, func update, func render, func input);
void window_loop(void);
void window_destroy(void);
extern struct Window window;
#endif
