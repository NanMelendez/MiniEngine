#type vertex

// CPU-defined
// ============
#version 330 core

// Vertex-exclusive definitions
// =============================
layout (location = 0) in vec3 _position;

out SHADER_DATA {
    vec3 position;
} data;

void MainVertex(inout vec3 vertPosition);

void main() {
    data.position = _position;
    MainVertex(data.position);
}

// Vertex-exclusive uniforms
// ==========================
layout (std140) uniform _uMatrices {
    mat4 P;
    mat4 V;
    mat4 cV; // Cubemap View
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
    gl_Position = (P * cV * vec4(vertPosition, 1.0)).xyww;
}

#type fragment

#version 330 core

in SHADER_DATA {
    vec3 position;
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
    samplerCube diffuse;
};

uniform Material material;

// Fragment entry point
// =====================
void MainFragment(inout vec4 fragColor) {
    fragColor = texture(material.diffuse, data.uv);
}