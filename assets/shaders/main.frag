#version 330 core

in vec3 vColor;
in vec2 vUVCoord;

out vec4 fColor;

uniform vec3 myColor;
uniform sampler2D myTexture;

void main() {
    fColor = texture(myTexture, vUVCoord) * vec4(vColor, 1.0);
}