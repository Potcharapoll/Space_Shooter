#include "shader.h"
#include "../state.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static GLuint _compile(char *path, GLenum type) {
    FILE *f;
    char *txt;
    s32 len;

    f = fopen(path, "rb");
    if (f == NULL) {
        fprintf(stderr, "Cannot open file at %s\n", path);
        exit(EXIT_FAILURE);
    }

    fseek(f, 0, SEEK_END);
    len = ftell(f);
    assert(len > 0);
    fseek(f, 0, SEEK_SET);
    txt = calloc(1, len);
    assert(txt != NULL);
    fread(txt, 1, len, f);
    assert(strlen(txt) > 0);

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, (const char* const*)&txt, (const GLint*)&len);
    glCompileShader(shader);
    
    s32 check;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &check);
    if (check == GL_FALSE) {
        char log[1024];
        glGetShaderInfoLog(shader, 1024, NULL, log);
        printf("Failed to compile %s Shader\n%s\n", type == GL_VERTEX_SHADER ? "Vertex" : "Fragment", log);
    }
    return shader;
}

struct Shader shader_create(char *vs_path, char *fs_path) {
    struct Shader shader;
    shader.handle = glCreateProgram();
    shader.vs = _compile(vs_path, GL_VERTEX_SHADER);
    shader.fs = _compile(fs_path, GL_FRAGMENT_SHADER);
    glAttachShader(shader.handle, shader.vs);
    glAttachShader(shader.handle, shader.fs);
    glLinkProgram(shader.handle);
    
    s32 check;
    glGetProgramiv(shader.handle, GL_LINK_STATUS, &check);
    if (check == GL_FALSE) {
        char log[1024];
        glGetProgramInfoLog(shader.handle, 1024, NULL, log);
        printf("Failed to link Shader Program\n%s\n", log);
    }

    glValidateProgram(shader.handle);
    return shader;
}

void shader_bind(struct Shader self) {
    glUseProgram(self.handle);
}

void shader_unbind(void) {
    glUseProgram(0);
}
void shader_uniform_proj(struct Shader self) {
   glUniformMatrix4fv(glGetUniformLocation(self.handle , "proj"), 1, GL_FALSE,  (const f32*)state.proj);
}

void shader_uniform_texture(struct Shader self, struct Texture tex) {
    glUniform1i(glGetUniformLocation(self.handle, "tex"), tex.unit);
}

void shader_uniform_mat4(struct Shader self, char *name, mat4 m) {
   glUniformMatrix4fv(glGetUniformLocation(self.handle , name), 1, GL_FALSE,  (const f32*)m);
}

void shader_uniform_vec3(struct Shader self, char *name, vec3 v) {
    glUniform3f(glGetUniformLocation(self.handle, name), v[0], v[1], v[2]);
}

void shader_uniform_float(struct Shader self, char *name, f32 f) {
    glUniform1f(glGetUniformLocation(self.handle, name), f); 
}

void shader_uniform_int(struct Shader self, char *name, s32 i) {
    glUniform1i(glGetUniformLocation(self.handle, name), i); 
}


void shader_delete(struct Shader self) {
    glDeleteShader(self.vs);
    glDeleteShader(self.fs);
    glDeleteProgram(self.handle);
}
