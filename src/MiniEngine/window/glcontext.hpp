#pragma once

#ifndef __MINIENGINE_GL_CONTEXT__
#define __MINIENGINE_GL_CONTEXT__

#include "rendercontext.hpp"

namespace MiniEngine {
    namespace Window {
        WindowBase* wCast(GLFWwindow* window) {
            return static_cast<WindowBase*>(glfwGetWindowUserPointer(window));
        }

        void keyCallback(GLFWwindow* window, i32 key, i32 scancode, i32 action, i32 mods) {
            WindowBase* _window = wCast(window);
            _window->keyCallback(key, scancode, action, mods);
        }

        void mouseButtonCallback(GLFWwindow* window, i32 button, i32 action, i32 mods) {
            WindowBase* _window = wCast(window);
            _window->mouseButtonCallback(button, action, mods);
        }

        void cursorPosCallback(GLFWwindow* window, f64 x, f64 y) {
            WindowBase* _window = wCast(window);
            _window->cursorPosCallback(x, y);
        }

        void scrollCallback(GLFWwindow* window, f64 x, f64 y) {
            WindowBase* _window = wCast(window);
            _window->scrollCallback(x, y);
        }

        void closeCallback(GLFWwindow* window) {
            WindowBase* _window = wCast(window);
            _window->closeCallback();
        }

        void framebufferSizeCallback(GLFWwindow* window, i32 width, i32 height) {
            WindowBase* _window = wCast(window);
            _window->framebufferSizeCallback(width, height);
        }

        class GLContext : public RenderContext {
        public:
            bool init(WindowBase* _window) override {
                RenderContext::init(_window);

                if (!glfwInit()) {
                    std::cerr << "Could not initialize GLFW." << std::endl;
                    return false;
                }

                GLFWwindow* glWindow = glfwCreateWindow(window->width, window->height, window->title.c_str(), NULL, NULL);
                window->setNativeWindow(glWindow);

                if (!glWindow) {
                    std::cerr << "Could not create GLFW window." << std::endl;
                    return false;
                }

                glfwMakeContextCurrent(glWindow);
                glfwSetKeyCallback(glWindow, keyCallback);
                glfwSetMouseButtonCallback(glWindow, mouseButtonCallback);
                glfwSetCursorPosCallback(glWindow, cursorPosCallback);
                glfwSetScrollCallback(glWindow, scrollCallback);
                glfwSetWindowCloseCallback(glWindow, closeCallback);
                glfwSetFramebufferSizeCallback(glWindow, framebufferSizeCallback);
                glfwSetWindowUserPointer(glWindow, _window);

                if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
                    std::cerr << "Failed to initialzie GLAD" << std::endl;
                    return false;
                }

                return true;
            }

            void preRender() override {
                glClearColor(window->color.r, window->color.g, window->color.b, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);
            }

            void postRender() override {
                glfwSwapBuffers((GLFWwindow*)window->getNativeWindow());
                glfwPollEvents();
            }

            void end() override {
                glfwDestroyWindow((GLFWwindow*)window->getNativeWindow());
                glfwTerminate();
            }

            void setIcon(const i8* path) {
                i32 nChannels;
                GLFWimage img;

                img.pixels = stbi_load(path, &img.width, &img.height, &nChannels, 0);

                glfwSetWindowIcon((GLFWwindow*)window->getNativeWindow(), 1, &img);
                
                stbi_image_free(img.pixels);
            }
        };
    }
}

#endif
