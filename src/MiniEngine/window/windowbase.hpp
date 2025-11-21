#pragma once

#ifndef __MINIENGINE_WINDOW_BASE__
#define __MINIENGINE_WINDOW_BASE__

#include "../pch.hpp"
#include "../extras/types.hpp"

namespace MiniEngine {
    namespace Window {
        class WindowBase {
        public:
            i32 width, height;
            std::string title;
            glm::vec3 color;

            virtual void* getNativeWindow() const = 0;

            virtual void setNativeWindow(void* window) = 0;

            virtual void keyCallback(i32 key, i32 scancode, i32 action, i32 mods) = 0;

            virtual void mouseButtonCallback(i32 button, i32 action, i32 mods) = 0;

            virtual void cursorPosCallback(f64 x, f64 y) = 0;

            virtual void scrollCallback(f64 x, f64 y) = 0;

            virtual void closeCallback() = 0;

            virtual void framebufferSizeCallback(i32 width, i32 height) = 0;

            // virtual void charCallback(u32 codepoint) = 0;

            // virtual void cursorEnterCallback(i32 entered) = 0;

            // virtual void dropCallback(i32 count, const i8* paths[]) = 0;

            // virtual void windowSizeCallback(i32 width, i32 height) = 0;

            // virtual void windowContentScaleCallback(f32 x, f32 y) = 0;

            // virtual void windowPosCallback(i32 x, i32 y) = 0;

            // virtual void windowIconifyCallback(i32 iconified) = 0;

            // virtual void windowMaximizeCallback(i32 maximized) = 0;

            // virtual void windowFocusCallback(i32 focused) = 0;

            // virtual void windowRefreshCallback() = 0;

            // virtual void monitorCallback(i32 event) = 0;
        };
    }
}

#endif
