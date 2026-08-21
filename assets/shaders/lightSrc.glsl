#type vertex

// CPU-defined
// ============
#version 330 core

// Vertex-exclusive definitions
// =============================
layout (location = 0) in vec3 _position;
layout (location = 1) in vec3 _normal;
layout (location = 2) in vec2 _uv;

out SHADER_DATA {
    vec3 position;
    vec3 normal;
    vec2 uv;
} data;

uniform mat4 M;
uniform mat3 mN;

void MainVertex(inout vec3 vertPosition);

void main() {
    data.position = _position;
    data.normal = mN * _normal;
    data.uv = _uv;
    MainVertex(data.position);
}

// Vertex-exclusive uniforms
// ==========================
layout (std140) uniform _uMatrices {
    mat4 P;
    mat4 V;
};

// Global uniforms
// ================
layout (std140) uniform _uGlobal {
    ivec2 resolution;
    float time;
};

// Vertex entry point
// ===================

void MainVertex(inout vec3 vertPosition) {
    vertPosition = vec3(M * vec4(vertPosition, 1.0));
    gl_Position = P * V * vec4(vertPosition, 1.0);
}

#type fragment

// CPU-defined (unlit version)
// ============================
#version 330 core

// Fragment-exclusive definitions
// ===============================
in SHADER_DATA {
    vec3 position;
    vec3 normal;
    vec2 uv;
} data;

out vec4 _fColor;

void MainFragment(inout vec4 fragColor);

void main() {
    _fColor = vec4(0.0, 0.0, 0.0, 1.0);
    MainFragment(_fColor);
}

// Global uniforms
// ================
layout (std140) uniform _uGlobal {
    ivec2 resolution;
    float time;
};

// User-defined code
// ==================
struct Material {
    vec3 color;
};

uniform Material material;

void MainFragment(inout vec4 fragColor) {
    fragColor = vec4(material.color, 1.0);
}
