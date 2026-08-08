#version 330 core

in SHADER_DATA {
    vec3 position;
    vec3 normal;
    vec2 uv;
} data;

out vec4 fColor;

uniform sampler2D myTexture;

void main() {
    fColor = texture(myTexture, data.uv);
}