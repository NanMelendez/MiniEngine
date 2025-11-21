#pragma once

#ifndef __MINIENGINE_WINDOW__
#define __MINIENGINE_WINDOW__

#include "windowbase.hpp"
#include "glcontext.hpp"
#include "../scene/scenemanager.hpp"

namespace MiniEngine {
    namespace Window {
        class Window : public WindowBase {
        public:
            Window() {
                renderCtx = new GLContext;
            }

            ~Window() {
                renderCtx->end();
            }

            bool init(i32 _width, i32 _height, const std::string& _title) {
                width = _width;
                height = _height;
                title = _title;
                color = glm::vec3(0.2f, 0.3f, 0.3f);

                renderCtx->init(this);
                renderCtx->setIcon("../assets/textures/miniengine_logo.png");

                return isRunning();
            }

            bool isRunning() const {
                return !glfwWindowShouldClose(window);
            }

            void* getNativeWindow() const override {
                return window;
            }

            void setNativeWindow(void* _window) override {
                window = (GLFWwindow*)_window;
            }

            void processInput() {
                if (glfwGetKey((GLFWwindow*)window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                    glfwSetWindowShouldClose((GLFWwindow*)window, true);
            }

            void render() {
                processInput();

                renderCtx->preRender();

                // Render current scene...

                renderCtx->postRender();
            }

            virtual void keyCallback(i32 key, i32 scancode, i32 action, i32 mods) override {}

            virtual void mouseButtonCallback(i32 button, i32 action, i32 mods) override {}

            virtual void cursorPosCallback(f64 x, f64 y) override {}

            virtual void scrollCallback(f64 x, f64 y) override {}

            virtual void closeCallback() override {}

            virtual void framebufferSizeCallback(i32 width, i32 height) override {
                glViewport(0, 0, width, height);
            }

            // void charCallback(u32 codepoint) override {}

            // void cursorEnterCallback(i32 entered) override {}

            // void dropCallback(i32 count, const i8* paths[]) override {}

            // void windowSizeCallback(i32 width, i32 height) override {}

            // void windowContentScaleCallback(f32 x, f32 y) override {}

            // void windowPosCallback(i32 x, i32 y) override {}

            // void windowIconifyCallback(i32 iconified) override {}

            // void windowMaximizeCallback(i32 maximized) override {}

            // void windowFocusCallback(i32 focused) override {}

            // void windowRefreshCallback() override {}

            // void monitorCallback(i32 event) override {}

        private:
            GLFWwindow* window;
            GLContext* renderCtx;


        };
    }
}

#endif
