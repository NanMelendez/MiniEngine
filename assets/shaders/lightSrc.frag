#version 330 core

in SHADER_DATA {
    vec3 position;
    vec3 normal;
    vec2 uv;
} data;

out vec4 fColor;

uniform vec3 lightColor;

void main() {
    fColor = vec4(lightColor, 1.0);
}