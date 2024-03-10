#version 330 core
out vec4 FragColor;

in vec2 vTex;

uniform sampler2D tex;

void main() {
    FragColor = vec4(0.5, 0.5, 1.0, texture(tex, vTex).r);
}
