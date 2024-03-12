#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "texture.h"

struct Texture texture_load(s32 unit, char *path, GLenum internal_format, GLenum source_format) {
    struct Texture texture = { .unit = unit };

    stbi_set_flip_vertically_on_load(1);
    s32 width = 0, height = 0, bbp = 0;
    u8 *pixels = stbi_load(path, &width, &height, &bbp, 0);
    if (pixels == NULL) {
        fprintf(stderr, "%s: %s\n", path, stbi_failure_reason());
        exit(EXIT_FAILURE);
    }

    glGenTextures(1, &texture.handle);
    glBindTexture(GL_TEXTURE_2D, texture.handle);
    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, source_format, GL_UNSIGNED_BYTE, pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(pixels);
    return texture;
}

void texture_bind(struct Texture texture) {
    glActiveTexture(GL_TEXTURE0+texture.unit);
    glBindTexture(GL_TEXTURE_2D, texture.handle);
}

void texture_unbind(void) {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void texture_delete(struct Texture texture) {
    glDeleteTextures(1, &texture.handle);
}
