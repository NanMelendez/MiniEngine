#ifndef __MINIENGINE_CAMERA__
#define __MINIENGINE_CAMERA__

#include "transform.hpp"

namespace MiniEngine {
    struct alignas(16) CameraRawData {
        glm::vec3 position;
        glm::vec3 front;
        f32 fov;
        glm::vec3 right;
        f32 zNear;
        glm::vec3 up;
        f32 zFar;
    };
    
    enum class CameraProjectionMode {
        PERSPECTIVE,
        ORTHOGRAPHIC
    };

    class Camera {
    public:
        Transform* transform;

        f32 fov;
        f32 zNear, zFar;

        CameraProjectionMode projectionMode = CameraProjectionMode::PERSPECTIVE;

        Camera(Transform* transform, f32 fov = 45.0f, f32 zNear = 0.1f, f32 zFar = 100.0f) : transform(transform), fov(fov), zNear(zNear), zFar(zFar) {}

        glm::mat4 projection(i32 width, i32 height) const {
            switch (projectionMode) {
            default:
            case CameraProjectionMode::PERSPECTIVE:
                return glm::perspective(glm::radians(fov), (f32)width / (f32)height, zNear, zFar);
            case CameraProjectionMode::ORTHOGRAPHIC:
                return glm::ortho(0.0f, (f32)width, 0.0f, (f32)height, zNear, zFar);
            }
        }

        CameraRawData getRawData() const {
            return {
                transform->position,
                transform->front(),
                fov,
                transform->right(),
                zNear,
                transform->up(),
                zFar
            };
        }
    };
}

#endif