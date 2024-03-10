#ifndef TEXT_H
#define TEXT_H
#include "texture.h"
#include "shader.h"
#include "vao.h"
#include "buffer.h"

#include <stdbool.h>
#include <cglm/cglm.h>

struct Text {
    struct VAO vao;
    struct Buffer vbo, ibo;
    struct Shader shader;
    struct Texture font_texture;
    
    float width, height;
    float char_width;
};

struct Text text_load(char *path, float width, float height, float char_width);
void text_render(struct Text self, const char *text, float x, float y, float scale, vec3 text_color);
void text_destroy(struct Text self);
#endif


 
