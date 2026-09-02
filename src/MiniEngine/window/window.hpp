#ifndef __MINIENGINE_WINDOW__
#define __MINIENGINE_WINDOW__

#include "../pch.hpp"
#include "../world/camera.hpp"
#include "../wrappers/fbo.hpp"

namespace MiniEngine {
    class Window {
    public:
        glm::vec3 bgColor;

        bool init(i32 _width, i32 _height, i8* title, glm::vec3 _bgColor = glm::vec3(1.0f)) {
            bgColor = _bgColor;
            width = _width;
            height = _height;

            window = glfwCreateWindow(_width, _height, title, NULL, NULL);

            if (!window) {
                std::cerr << "Failed to create GLFW window." << std::endl;
                glfwTerminate();
                return -1;
            }

            glfwMakeContextCurrent(window);
            glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
            glfwSetScrollCallback(window, scrollCallback);
            glfwSetCursorPosCallback(window, cursorPosCallback);
            glfwSetMouseButtonCallback(window, mouseButtonCallback);

            if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
                std::cerr << "Failed to initialize GLAD" << std::endl;
                return -1;
            }

            glEnable(GL_DEPTH_TEST);
            glEnable(GL_CULL_FACE);
        }

        GLFWwindow* getNative() const {
            return window;
        }

        glm::ivec2 getResolution() const {
            return glm::ivec2(width, height);
        }

        void setTitle(const i8* title) {
            glfwSetWindowTitle(window, title);
        }

        const i8* getTitle() const {
            return glfwGetWindowTitle(window);
        }

    private:
        GLFWwindow* window;
        FBO* fbo;

        i32 width, height;
        Camera* activeCamera;

        bool holdingCamera = false;
        i32 lastX, lastY;

        f32 cursorSensitivity = 0.005f;

        void framebufferSizeCallback(GLFWwindow* window, i32 _width, i32 _height) {
            glViewport(0, 0, _width, _height);
            width = _width;
            height = _height;
            fbo->resize(width, height);
        }

        void scrollCallback(GLFWwindow* window, f64 xOffset, f64 yOffset) {
            activeCamera->fov -= yOffset;

            if (activeCamera->fov < 0.1f)
                activeCamera->fov = 0.1f;
            if (activeCamera->fov > 89.0f)
                activeCamera->fov = 89.0f;
        }

        void cursorPosCallback(GLFWwindow* window, f64 x, f64 y) {
            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
                if (!holdingCamera) {
                    lastX = x;
                    lastY = y;
                    holdingCamera = true;
                }

                f32 xOffset = x - lastX;
                f32 yOffset = lastY - y;

                lastX = x;
                lastY = y;

                xOffset *= cursorSensitivity;
                yOffset *= cursorSensitivity;

                glm::quat yaw = glm::angleAxis(-xOffset, glm::vec3(0.0f, 1.0f, 0.0f));
                glm::quat pitch = glm::angleAxis(yOffset, glm::vec3(1.0f, 0.0f, 0.0f));

                activeCamera->transform->rotation = glm::normalize(yaw * activeCamera->transform->rotation * pitch);
            }
            else
                holdingCamera = false;
        }

        void mouseButtonCallback(GLFWwindow* window, i32 button, i32 action, i32 mods) {
            if (button == GLFW_MOUSE_BUTTON_LEFT) {
                if  (action == GLFW_PRESS)
                    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                if (action == GLFW_RELEASE)
                    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
        }
    };
}

#endif