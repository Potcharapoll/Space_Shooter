#ifndef TEXT_H
#define TEXT_H
#include "texture.h"
#include "shader.h"
#include "vao.h"
#include "buffer.h"
#include "../util/util.h"


struct Text {
    struct VAO vao;
    struct Buffer vbo, ibo;
    struct Shader shader;
    struct Texture font_texture;
    
    vec2 size;
    f32 char_width;
};

struct Text text_load(char *path, vec2 size, f32 char_width);
void text_render(struct Text self, const char *text, vec2 position, f32 scale, vec3 text_color);
void text_blink_render(struct Text self, const char *text, vec2 position, f32 scale, vec3 text_color, f32 time, f32 *timer);
void text_destroy(struct Text self);
#endif


 
