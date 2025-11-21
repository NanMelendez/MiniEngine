#pragma once

#ifndef __MINIENGINE_APP_MAIN__
#define __MINIENGINE_APP_MAIN__

#include "window/window.hpp"

namespace MiniEngine {
    class Application {
    public:
        Application(i32 width, i32 height, const std::string& title) {
            window = new Window::Window;
            window->init(width, height, title);
        }

        static Application& Instance() {
            return *instance;
        }

        void run() {
            while (window->isRunning())
                window->render();
        }

    private:
        static Application* instance;

        Window::Window* window;
    };
}

#endif
