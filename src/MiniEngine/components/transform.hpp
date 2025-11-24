#pragma once

#ifndef __MINIENGINE_TRANSFORM__
#define __MINIENGINE_TRANSFORM__

#include "component.hpp"

namespace MiniEngine {
    namespace Components {
        enum class TransformOperationOrder {
            TRS, TSR, RTS, RST, STR, SRT
        };

        class Transform : public Component {
        public:
            TransformOperationOrder operationOrder = TransformOperationOrder::SRT;

            Transform(const glm::vec3& _position, const glm::quat& _rotation, const glm::vec3& _localScale, SceneObject* _sceneObject) : Component(_sceneObject), position(_position), rotation(_rotation), localScale(_localScale) {}

            glm::vec3 position;
            glm::quat rotation;
            glm::vec3 localScale;

            Transform* parent = nullptr;
            std::vector<Transform*> children;

            void translate(glm::vec3 p) {
                position += p;
            }

            void rotate(glm::quat r) {
                rotation *= r;
            }

            void scale(glm::vec3 s) {
                localScale *= s;
            }

            void lookAt(const Transform& other, glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f)) {
                glm::vec3 direction = glm::normalize(other.position - position);
                f32 dp = glm::dot(worldUp, direction);

                if (glm::length(direction) == 0.0f || dp == -1.0f || dp == 1.0f)
                    return;
                
                rotation = glm::lookAt(position, position + direction, worldUp);
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

            glm::mat4 model() const {
                glm::mat4 tr = glm::translate(glm::mat4(1.0f), position);
                glm::mat4 rt = glm::mat4_cast(glm::normalize(rotation));
                glm::mat4 sc = glm::scale(glm::mat4(1.0f), localScale);

                switch (operationOrder) {
                default:
                case TransformOperationOrder::TRS: return sc * rt * tr;
                case TransformOperationOrder::TSR: return rt * sc * tr;
                case TransformOperationOrder::RTS: return sc * tr * rt;
                case TransformOperationOrder::RST: return tr * sc * rt;
                case TransformOperationOrder::STR: return rt * tr * sc;
                case TransformOperationOrder::SRT: return tr * rt * sc;
                }
            }

            glm::mat4 view() const {
                return glm::lookAt(position, position + front(), up());
            }
            
            MINIENGINE_DEFCLASSNAME(Transform)
        };
    }
}

#endif