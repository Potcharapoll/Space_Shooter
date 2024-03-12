#include "text.h"
#include "window.h"
#include "../state.h"

struct Text text_load(char *path, vec2 size, float char_width) {
    struct Text txt = {
        .vao = vao_create(),
        .vbo = buffer_create(GL_ARRAY_BUFFER, false),
        .ibo = buffer_create(GL_ELEMENT_ARRAY_BUFFER, false),
        .shader = shader_create("../res/shaders/text.vert", "../res/shaders/text.frag"),
        .font_texture = texture_load(0, path, GL_RED, GL_RGBA),
        .char_width = char_width,
    };
    glm_vec2_copy(size, txt.size);
    unsigned int indices[] = { 0,1,2,2,3,0 };

    vao_bind(txt.vao);
    buffer_bind(txt.vbo);
    buffer_data(txt.vbo, 4*4*sizeof(float), NULL);
    vao_attr(0, 4, GL_FLOAT, 4*sizeof(float), 0);
    
    buffer_bind(txt.ibo);
    buffer_data(txt.ibo, sizeof(indices), indices);

    shader_bind(txt.shader);
    shader_uniform_proj(txt.shader);
    shader_uniform_float(txt.shader, "width", txt.size[0]);
    shader_uniform_float(txt.shader, "height", txt.size[1]);
    shader_uniform_texture(txt.shader, txt.font_texture);
    shader_unbind();

    vao_unbind();
    buffer_unbind(txt.vbo);
    buffer_unbind(txt.ibo);
    return txt;
}

void text_render(struct Text self, const char *text, vec2 position, f32 scale, vec3 text_color) {
    
    f32 offset_x, offset_y;
    f32 char_width = self.char_width;

    f32 xpos = position[0];
    f32 ypos = position[1];

    f32 w = self.char_width*scale;
    f32 h = self.char_width*scale;

    s32 rows    = 0; 
    s32 columns = 0;

    shader_bind(self.shader);
    shader_uniform_vec3(self.shader, "text_color", text_color);
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
        offset_y = self.size[1] - char_width - (char_width*columns);

        f32 vertices[] = {
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

void text_blink_render(struct Text self, const char *text, vec2 position, f32 scale, vec3 text_color, f32 time, f32 *timer) {
    *timer += window.delta_time;
    if (*timer > 0.0f && *timer < time) {
        text_render(self, text, position, scale, text_color);
    }
    else if (*timer > time) {
        *timer = -fabsf(*timer);
    }  
}

void text_destroy(struct Text self) {
    texture_delete(self.font_texture);
    shader_delete(self.shader);
    vao_delete(self.vao);
    buffer_delete(self.vbo);
    buffer_delete(self.ibo);
}
