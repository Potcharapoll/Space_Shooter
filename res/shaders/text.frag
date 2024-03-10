#version 330 core
out vec4 FragColor;

uniform sampler2D text;
uniform vec3 text_color;

in vec2 vTex;

void main() {
    FragColor = vec4(text_color, texture(text, vTex).r);
}
