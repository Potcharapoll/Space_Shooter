#ifndef STATE_H
#define STATE_H
#include "util/util.h"

struct State {
    mat4 proj;

    enum {
        MENU=0,
        INGAME,
        PAUSE,
        GAMEOVER
    }state;
};
extern struct State state;
#endif
