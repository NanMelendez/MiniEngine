#version 330 core

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

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;
uniform vec3 viewPos;
uniform float time;

float grayscale(vec3 color) {
    return 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
}

void main() {
    // Ambient
    vec3 ambient = light.ambient * texture(material.diffuse, data.uv).rgb;

    // Diffuse
    vec3 norm = normalize(data.normal);
    vec3 lightDir = normalize(light.position - data.position);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, data.uv).rgb;
    
    // Specular
    vec3 viewDir = normalize(viewPos - data.position);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, data.uv).rgb;

    // Emissive
    vec3 emissive = texture(material.emissive, data.uv).rgb * material.emissiveStrenght * (0.5 + 0.5 + sin(time * 2.0));

    vec3 result = ambient + diffuse + specular + mix(emissive, vec3(0.0), ceil(grayscale(texture(material.specular, data.uv).rgb)));

    fColor = vec4(result, 1.0);
}