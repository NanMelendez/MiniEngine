#ifndef __MINIENGINE_TRANSFORM__
#define __MINIENGINE_TRANSFORM__

#include "../pch.hpp"
#include "../core/types.hpp"

namespace MiniEngine {
    enum class TransformOrder {
        TRS, TSR, RTS, RST, STR, SRT
    };

    class Transform {
    public:
        glm::vec3 position;
        glm::quat rotation;
        glm::vec3 localScale;

        TransformOrder order = TransformOrder::SRT;

        Transform() : position(glm::vec3(0.0f)), rotation(glm::identity<glm::quat>()), localScale(glm::vec3(1.0f)) {}

        Transform(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& localScale) : position(position), rotation(rotation), localScale(localScale) {}

        Transform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& localScale) : position(position), rotation(glm::quat(rotation)), localScale(localScale) {}

        void translate(glm::vec3 tr) {
            position += tr;
        }

        void rotate(glm::quat rt) {
            rotation *= rt;
        }

        void scale(glm::vec3 sc) {
            localScale *= sc;
        }

        glm::vec3 front() const {
            return glm::normalize(rotation * glm::vec3(0.0f, 0.0f, -1.0f));
        }

        glm::vec3 right() const {
            return glm::normalize(rotation * glm::vec3(1.0f, 0.0f, 0.0f));
        }

        glm::vec3 up() const {
            return glm::normalize(rotation * glm::vec3(0.0f, 1.0f, 0.0f));
        }

        glm::vec3 eulerAngles() const {
            return glm::eulerAngles(rotation);
        }

        void lookAt(const Transform& other, glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f)) {
            lookAt(other.position, worldUp);
        }

        void lookAt(const glm::vec3& position, glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f)) {
            glm::vec3 direction = glm::normalize(position - this->position);
            f32 dprod = glm::dot(worldUp, direction);

            if (glm::length(direction) == 0.0f || glm::abs(dprod) == 1.0f)
                return;
            
            rotation = glm::quatLookAt(direction, worldUp);
        }

        glm::mat4 view() const {
            return glm::lookAt(position, position + front(), up());
        }

        glm::mat4 model() const {
            glm::mat4 tr = glm::translate(glm::mat4(1.0f), position);
            glm::mat4 rt = glm::mat4_cast(glm::normalize(rotation));
            glm::mat4 sc = glm::scale(glm::mat4(1.0f), localScale);

            switch (order) {
            case TransformOrder::TRS: return sc * rt * tr;
            case TransformOrder::TSR: return rt * sc * tr;
            case TransformOrder::RTS: return sc * tr * rt;
            case TransformOrder::RST: return tr * sc * rt;
            case TransformOrder::STR: return rt * tr * sc;
            default:
            case TransformOrder::SRT: return tr * rt * sc;
            }
        }
    };
}

#endif