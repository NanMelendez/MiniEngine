#pragma once

#ifndef __MINIENGINE_CORE_VERTEX_ARRAY_OBJECT__
#define __MINIENGINE_CORE_VERTEX_ARRAY_OBJECT__

#include "coreContainer.hpp"

namespace MiniEngine {
    namespace Core {
        class VAO : public GLContainer {
        public:
            VAO() : GLContainer() { load(); }

            void bind() const override {
                glBindVertexArray(id);
            }

            void unbind() const override {
                glBindVertexArray(0);
            }

            void load() {
                clear();
                glGenVertexArrays(1, &id);
            }

            void clear() override {
                if (id) {
                    glDeleteVertexArrays(1, &id);
                    id = 0;
                }
            }

            void setAttribPointer(GLuint idx, GLint size, GLsizei stride, const GLvoid* ptr, GLenum type = GL_FLOAT, GLboolean normalized = GL_FALSE) const {
                glVertexAttribPointer(idx, size, type, normalized, stride, ptr);
            }

            void enableIndex(GLuint idx) const {
                glEnableVertexAttribArray(idx);
            }

            void disableIndex(GLuint idx) const {
                glDisableVertexAttribArray(idx);
            }
        };
    }
}

#endif
