#version 330 core

in SHADER_DATA {
    vec3 position;
    vec3 normal;
    vec2 uv;
} data;

out vec4 fColor;

struct Material {
    sampler2D targetTexture;
};

uniform Material material;

layout (std140) uniform _uGlobal {
    ivec2 resolution;
    float time;
};

vec3 invert(vec3 color) {
    return 1.0 - color;
}

void main() {
    fColor = vec4(invert(texture(material.targetTexture, data.uv).rgb), 1.0);
}