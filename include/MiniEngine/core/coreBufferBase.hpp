#pragma once

#ifndef __MINIENGINE_CORE_BUFFERBASE__
#define __MINIENGINE_CORE_BUFFERBASE__

#include "coreContainer.hpp"

namespace MiniEngine {
    namespace Core {
        class Buffer : public Container {
        public:
            Buffer() : Container() {}

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
