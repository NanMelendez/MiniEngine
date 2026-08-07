#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 uvCoord;

out vec3 vColor;
out vec2 vUVCoord;

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

void main() {
    gl_Position = P * V * M * vec4(position, 1.0);
    vColor = color;
    vUVCoord = uvCoord;
}