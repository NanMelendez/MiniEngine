#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

out SHADER_DATA {
    vec3 position;
    vec3 normal;
    vec2 uv;
} data;

/*
layout (std140) uniform _uMatrices {
    mat4 P;
    mat4 V;
};
*/

uniform mat4 P;
uniform mat4 V;

uniform mat4 M;
uniform mat3 mN;

void main() {
    data.position = vec3(M * vec4(position, 1.0));
    data.normal = mN * normal;
    data.uv = uv;

    gl_Position = P * V * vec4(data.position, 1.0);
}