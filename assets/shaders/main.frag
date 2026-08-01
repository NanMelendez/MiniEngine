#version 330 core

in vec3 vColor;

out vec4 fColor;

uniform vec3 myColor;

void main() {
    fColor = vec4(myColor, 1.0);
}