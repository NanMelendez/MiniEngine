#type vertex

#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

out SHADER_DATA {
    vec3 position;
    vec3 normal;
    vec2 uv;
} data;

layout (std140) uniform _uGlobal {
    ivec2 resolution;
    float time;
};

void main() {
    data.position = position;
    data.normal = normal;
    data.uv = uv;

    gl_Position = vec4(data.position, 1.0);
}

#type fragment

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
    fColor = texture(material.targetTexture, data.uv);
}