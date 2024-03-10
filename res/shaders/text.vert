#version 330 core
layout (location = 0) in vec4 aPos;  // <vec2 pos, vec2 tex_pos>

uniform float width;
uniform float height;
uniform mat4 proj;

out vec2 vTex;

void main() {
    gl_Position = proj*vec4(aPos.xy, 0.0, 1.0);
    vTex = vec2(aPos.z/width, aPos.w/height);
}
