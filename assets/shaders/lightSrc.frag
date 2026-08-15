#version 330 core

in SHADER_DATA {
    vec3 position;
    vec3 normal;
    vec2 uv;
} data;

out vec4 fColor;

struct Material {
    vec3 color;
};

uniform Material material;

void main() {
    fColor = vec4(material.color, 1.0);
}