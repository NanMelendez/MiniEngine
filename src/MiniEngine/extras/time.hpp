#ifndef __MINIENGINE_TIME_MANAGER__
#define __MINIENGINE_TIME_MANAGER__

#include "../pch.hpp"
#include "types.hpp"

namespace MiniEngine {
    class Time {
    public:
        static f32 delta() {
            return deltaTime;
        }
        
        static f32 now() {
            return currentFrame;
        }

        static void update() {
            currentFrame = (f32)glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;
        }

        static f64 getFPS() {
            return 1.0 / (f64)deltaTime;
        }

    private:
        inline static f32 deltaTime;
        inline static f32 currentFrame = 0.0f;
        inline static f32 lastFrame = 0.0f;
    };
}

#endif