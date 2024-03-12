#ifndef SHADER_H
#define SHADER_H
#include "gfx.h"
#include "texture.h"
#include "../util/types.h"

#include <cglm/cglm.h>

struct Shader {
    GLuint handle, vs, fs;
};

struct Shader shader_create(char *vs_path, char *fs_path);
void shader_bind(struct Shader self);
void shader_unbind(void);
void shader_delete(struct Shader self);
void shader_uniform_proj(struct Shader self);
void shader_uniform_mat4(struct Shader self, char *name, mat4 m);
void shader_uniform_float(struct Shader self, char *name, f32 f);
void shader_uniform_vec3(struct Shader self, char *name, vec3 v);
void shader_uniform_int(struct Shader self, char *name, s32 i);
void shader_uniform_texture(struct Shader self, struct Texture tex);
#endif
