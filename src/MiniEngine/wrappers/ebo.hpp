#ifndef __MINIENGINE_ELEMENT_BUFFER_OBJECT__
#define __MINIENGINE_ELEMENT_BUFFER_OBJECT__

#include "../core/basebuffer.hpp"

namespace MiniEngine {
    class EBO final : public Core::BaseBuffer {
    public:
        EBO() {}

        EBO(GLsizeiptr size, const GLvoid* data, GLenum usage = GL_STATIC_DRAW) {
            load(size, data, usage);
        }

        u64 getCount() const {
            return count;
        }

        void bind() const override final {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
        }

        void unbind() const override final {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }

        void load(GLsizeiptr size, const GLvoid* data) override final {
            load(size, data, GL_STATIC_DRAW);
        }

        void load(GLsizeiptr size, const GLvoid* data, GLenum usage) {
            clear();
            glGenBuffers(1, &id);
            count = (u64)size / sizeof(u32);
            bind();
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);
            unbind();
        }

        void update(GLsizeiptr size, const GLvoid* data, GLintptr offset) const override final {
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
        }
        
        void reallocate(GLsizeiptr size, const GLvoid* data) const override final {
            reallocate(size, data, GL_STATIC_DRAW);
        }

        void reallocate(GLsizeiptr size, const GLvoid* data, GLenum usage) const {
            if (isAllocated()) {
                bind();
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);
                unbind();
            }
        }

    private:
        u64 count = 0;
    };
}

#endif