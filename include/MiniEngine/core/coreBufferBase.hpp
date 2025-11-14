#pragma once

#ifndef __MINIENGINE_CORE_BUFFER_BASE__
#define __MINIENGINE_CORE_BUFFER_BASE__

#include "coreContainer.hpp"

namespace MiniEngine {
    namespace Core {
        class Buffer : public GLContainer {
        public:
            Buffer() : GLContainer() {}

            virtual void load(GLsizeiptr size, const GLvoid* data) = 0;

            void clear() override {
                if (id) {
                    glDeleteBuffers(1, &id);
                    id = 0;
                }
            }
        };
    }
}

#endif
