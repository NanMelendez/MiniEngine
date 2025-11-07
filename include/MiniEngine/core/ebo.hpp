#pragma once

#ifndef __MINIENGINE_CORE_ELEMENTBUFFEROBJECT__
#define __MINIENGINE_CORE_ELEMENTBUFFEROBJECT__

#include "coreBufferBase.hpp"

namespace MiniEngine {
    namespace Core {
        class EBO : public Buffer {
        public:
            EBO() : Buffer() {}

            EBO(GLsizeiptr size, const GLvoid* data, GLenum usage = GL_STATIC_DRAW) {
                load(size, data, usage);
            }

            void bind() const {
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
            }

            void unbind() const {
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            }

            void load(GLsizeiptr size, const GLvoid* data) override {
                load(size, data, GL_STATIC_DRAW);
            }

            void load(GLsizeiptr size, const GLvoid* data, GLenum usage) {
                clear();
                glGenBuffers(1, &id);
                bind();
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);
                unbind();
            }
        };
    }
}

#endif
