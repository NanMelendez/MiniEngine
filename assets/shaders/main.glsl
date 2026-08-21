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

layout (std140) uniform _uMatrices {
    mat4 P;
    mat4 V;
};

layout (std140) uniform _uGlobal {
    ivec2 resolution;
    float time;
};

uniform mat4 M;
uniform mat3 mN;

void main() {
    data.position = vec3(M * vec4(position, 1.0));
    data.normal = mN * normal;
    data.uv = uv;

    gl_Position = P * V * vec4(data.position, 1.0);
}

#type fragment

#version 330 core

#define MAX_LIGHT_SOURCES 24
#define INVALID_LIGHT_TYPE     0
#define LIGHT_TYPE_DIRECTIONAL 1
#define LIGHT_TYPE_POINT       2
#define LIGHT_TYPE_SPOT        3

in SHADER_DATA {
    vec3 position;
    vec3 normal;
    vec2 uv;
} data;

out vec4 fColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emissive;
    float emissiveStrenght;
    float shininess;
};

struct Light {
    vec3 position;
    int lightType;
    vec3 direction;
    float constant;
    vec3 ambient;
    float linear;
    vec3 diffuse;
    float quadratic;
    vec3 specular;
    float cutOff;
    float outerCutOff;
};

struct Camera {
    vec3 position;
    float zNear;
    float zFar;
};

uniform Material material;

layout (std140) uniform _uCamera {
    Camera camera;
};

layout (std140) uniform _uLights {
    Light lights[MAX_LIGHT_SOURCES];
};

layout (std140) uniform _uGlobal {
    ivec2 resolution;
    float time;
};

uniform float blinkOffset;

float grayscale(vec3 color) {
    return 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
}

vec3 calcDirectionalLights(Light light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, data.uv));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, data.uv));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, data.uv));

    return ambient + diffuse + specular;
}

vec3 calcPointLights(Light light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float dist = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, data.uv));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, data.uv));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, data.uv));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}

vec3 calcSpotLights(Light light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float dist = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * pow(dist, 2.0));

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, data.uv));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, data.uv));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, data.uv));

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return ambient + diffuse + specular;
}

void main() {
    vec3 norm = normalize(data.normal);
    vec3 viewDir = normalize(camera.position - data.position);

    vec3 result = vec3(0.0);

    for (int i = 0; i < MAX_LIGHT_SOURCES; i++) {
        if (lights[i].lightType == INVALID_LIGHT_TYPE)
            continue;

        if (lights[i].lightType == LIGHT_TYPE_DIRECTIONAL)
            result += calcDirectionalLights(lights[i], norm, viewDir);
        if (lights[i].lightType == LIGHT_TYPE_POINT)
            result += calcPointLights(lights[i], norm, data.position, viewDir);
        if (lights[i].lightType == LIGHT_TYPE_SPOT)
            result += calcSpotLights(lights[i], norm, data.position, viewDir);
    }
    
    vec3 emissive = texture(material.emissive, data.uv + vec2(0.0, 0.25 * time)).rgb * material.emissiveStrenght * (0.5 + 0.5 * sin(blinkOffset + (time * 2.0)));

    result += mix(emissive, vec3(0.0), ceil(grayscale(texture(material.specular, data.uv).rgb)));

    fColor = vec4(result, 1.0);
}