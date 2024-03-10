#include "text.h"
#include "../state.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct Text text_load(char *path, float width, float height, float char_width) {
    struct Text txt = {
        .vao = vao_create(),
        .vbo = buffer_create(GL_ARRAY_BUFFER, false),
        .ibo = buffer_create(GL_ELEMENT_ARRAY_BUFFER, false),
        .shader = shader_create("../res/shaders/text.vert", "../res/shaders/text.frag"),
        .font_texture = texture_load(0, path, GL_RED, GL_RGBA),
        .char_width = char_width,
        .width = width,
        .height = height,
    };
    unsigned int indices[] = { 0,1,2,2,3,0 };

    vao_bind(txt.vao);
    buffer_bind(txt.vbo);
    buffer_data(txt.vbo, 4*4*sizeof(float), NULL);
    vao_attr(0, 4, GL_FLOAT, 4*sizeof(float), 0);
    
    buffer_bind(txt.ibo);
    buffer_data(txt.ibo, sizeof(indices), indices);

    shader_bind(txt.shader);

    glUniformMatrix4fv(glGetUniformLocation(txt.shader.handle ,"proj"), 1, GL_FALSE,  (const float*)state.proj);
    glUniform1f(glGetUniformLocation(txt.shader.handle, "width"), txt.width);
    glUniform1f(glGetUniformLocation(txt.shader.handle, "height"), txt.height);
    glUniform1i(glGetUniformLocation(txt.shader.handle, "text"), txt.font_texture.unit);

    vao_unbind();
    buffer_unbind(txt.vbo);
    buffer_unbind(txt.ibo);
    shader_unbind();
    return txt;
}

void text_render(struct Text self, const char *text, float x, float y, float scale, vec3 text_color) {
    
    float offset_x, offset_y;
    float char_width = self.char_width;

    float xpos = x;
    float ypos = y;

    float w = self.char_width*scale;
    float h = self.char_width*scale;

    int rows    = 0; 
    int columns = 0;

    shader_bind(self.shader);
    glUniform3f(glGetUniformLocation(self.shader.handle, "text_color"), text_color[0], text_color[1], text_color[2]);

    texture_bind(self.font_texture);
    vao_bind(self.vao);

    for (const char *c = text; *c; ++c) {

        if (isalpha(*c) || isspace(*c)) {
            rows = *c - 'A';
            columns = 0;

            if (rows >= 8) {
                columns = floor(rows / 8);
                rows %= 8;
            }

        }
        else if (isdigit(*c)) {
            rows = *c - '0';
            columns = 3;

            if (rows > 5) {
                columns = 4;
            }

            rows = (2 + rows) % 8;
        }

        offset_x = char_width*rows;
        offset_y = self.height - char_width - (char_width*columns);

        float vertices[] = {
            xpos,     ypos,         offset_x,              offset_y,
            xpos + w, ypos,         offset_x + char_width, offset_y,
            xpos + w, ypos + h,     offset_x + char_width, offset_y + char_width,
            xpos,     ypos + h,     offset_x,              offset_y + char_width
        }; 

        buffer_bind(self.vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        buffer_unbind(self.vbo);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); 
        xpos += (char_width*scale);
    } 
    vao_unbind();
    texture_unbind();
}

void text_destroy(struct Text self) {
    texture_delete(self.font_texture);
    shader_delete(self.shader);
    vao_delete(self.vao);
    buffer_delete(self.vbo);
    buffer_delete(self.ibo);
}
