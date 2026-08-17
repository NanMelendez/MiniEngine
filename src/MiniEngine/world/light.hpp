#ifndef __MINIENGINE_LIGHT_SOURCE__
#define __MINIENGINE_LIGHT_SOURCE__

#include "transform.hpp"

namespace MiniEngine {
    enum class LightType : i32 {
        DIRECTIONAL = 1,
        POINT       = 2,
        SPOT        = 3
    };

    class LightSource {
    public:
        Transform* transform;

        LightType type;

        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;

        f32 constant;
        f32 linear;
        f32 quadratic;

        f32 cutOff; // Inner CutOff
        f32 outerCutOff; // Outer CutOff

        LightSource(Transform* transform, LightType type) : transform(transform), type(type) {
            ambient = glm::vec3(1.0f);
            diffuse = glm::vec3(1.0f);
            specular = glm::vec3(1.0f);

            constant = 1.0f;
            linear = 0.045f;
            quadratic = 0.0075f;

            cutOff = 0.0f;
            outerCutOff = 45.0f;
        }

        LightSource(Transform* transform, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) : transform(transform), type(LightType::DIRECTIONAL),
            ambient(ambient), diffuse(diffuse), specular(specular) {
            constant = 1.0f;
            linear = 0.045f;
            quadratic = 0.0075f;

            cutOff = 0.0f;
            outerCutOff = 45.0f;
        }

        LightSource(Transform* transform, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, f32 constant, f32 linear, f32 quadratic) : transform(transform), type(LightType::POINT),
            ambient(ambient), diffuse(diffuse), specular(specular),
            constant(constant), linear(linear), quadratic(quadratic) {
            cutOff = 0.0f;
            outerCutOff = 45.0f;
        }

        LightSource(Transform* transform, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, f32 constant, f32 linear, f32 quadratic, f32 cutOff, f32 outerCutOff) : transform(transform), type(LightType::SPOT),
            ambient(ambient), diffuse(diffuse), specular(specular),
            constant(constant), linear(linear), quadratic(quadratic),
            cutOff(cutOff), outerCutOff(outerCutOff) {

        }

        glm::vec3 direction() const {
            return transform->front();
        }

        /*
        f32 cutOff() const {
            return glm::radians(glm::mix(spotAngle, 0.0f, spotBlend));
        }

        f32 outerCutOff() const {
            return glm::radians(spotAngle);
        }
        */
    };
}

#endif