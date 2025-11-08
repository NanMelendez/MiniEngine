#pragma once

#ifndef __MINIENGINE_WINDOW_RENDER_CONTEXT__
#define __MINIENGINE_WINDOW_RENDER_CONTEXT__

#include "windowbase.hpp"

namespace MiniEngine {
    namespace Window {
        class RenderContext {
        public:
            RenderContext() : window(nullptr) {}
            
            virtual bool init(WindowBase* _window) {
                window = _window;
                return true;
            }

            virtual void preRender() = 0;

            virtual void postRender() = 0;

            virtual void end() = 0;

        protected:
            WindowBase* window;
        };
    }
};

#endif
