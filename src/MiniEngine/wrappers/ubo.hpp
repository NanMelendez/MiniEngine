#ifndef __MINIENGINE_UNIFORM_BUFFER_OBJECT__
#define __MINIENGINE_UNIFORM_BUFFER_OBJECT__

#include "../core/basebuffer.hpp"

namespace MiniEngine {
    class UBO : public Core::BaseBuffer {
    public:
        UBO() {}

        UBO(GLsizeiptr size) {
            load(size);
        }

        void bind() const override final {
            glBindBuffer(GL_UNIFORM_BUFFER, id);
        }

        void unbind() const override final {
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
        }

        void load(GLsizeiptr size, const GLvoid* data = NULL) override final {
            load(size, data, GL_STATIC_DRAW);
        }

        void load(GLsizeiptr size, const GLvoid* data, GLenum usage) {
            clear();
            glGenBuffers(1, &id);
            bind();
            glBufferData(GL_UNIFORM_BUFFER, size, data, usage);
            unbind();
        }

        void linkBase(i32 location) const {
            glBindBufferBase(GL_UNIFORM_BUFFER, location, id);
        }

        void linkRange(i32 location, i32 offset, i32 range) const {
            glBindBufferRange(GL_UNIFORM_BUFFER, location, id, offset, range);
        }

        void update(GLsizeiptr size, const GLvoid* data, GLintptr offset) const override final {
            glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
        }
        
        void reallocate(GLsizeiptr size, const GLvoid* data) const override final {
            reallocate(size, data, GL_STATIC_DRAW);
        }

        void reallocate(GLsizeiptr size, const GLvoid* data, GLenum usage) const {
            if (isAllocated()) {
                bind();
                glBufferData(GL_UNIFORM_BUFFER, size, data, usage);
                unbind();
            }
        }
    };
}

#endif