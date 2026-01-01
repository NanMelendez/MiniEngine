#pragma once

#ifndef __MINIENGINE_CORE_VERTEX_BUFFER_OBJECT__
#define __MINIENGINE_CORE_VERTEX_BUFFER_OBJECT__

#include "coreBufferBase.hpp"

namespace MiniEngine {
    namespace Core {
        class VBO final : public GLBuffer {
        public:
            VBO() : GLBuffer() {}

            VBO(GLsizeiptr size, const GLvoid* data, GLenum usage = GL_STATIC_DRAW) {
                load(size, data, usage);
            }

            void bind() const override final {
                glBindBuffer(GL_ARRAY_BUFFER, id);
            }

            void unbind() const override final {
                glBindBuffer(GL_ARRAY_BUFFER, 0);
            }

            void load(GLsizeiptr size, const GLvoid* data) override final {
                load(size, data, GL_STATIC_DRAW);
            }

            void load(GLsizeiptr size, const GLvoid* data, GLenum usage) {
                clear();
                glGenBuffers(1, &id);
                bind();
                glBufferData(GL_ARRAY_BUFFER, size, data, usage);
                unbind();
            }
        };
    }
}

#endif
