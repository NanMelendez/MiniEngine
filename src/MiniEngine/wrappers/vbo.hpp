#ifndef __MINIENGINE_VERTEX_BUFFER_OBJECT__
#define __MINIENGINE_VERTEX_BUFFER_OBJECT__

#include "../core/basebuffer.hpp"

namespace MiniEngine {
    class VBO final : public Core::BaseBuffer {
    public:
        VBO() {}

        VBO(GLsizeiptr size, const GLvoid* data, GLenum usage = GL_STATIC_DRAW) {
            load(size, data, usage);
        }

        void bind() const override final {
            glBindBuffer(GL_ARRAY_BUFFER, id);
            currentlyUsedID = id;
        }

        void unbind() const override final {
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            currentlyUsedID = 0;
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

        void update(GLsizeiptr size, const GLvoid* data, GLintptr offset) const override final {
            glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
        }
        
        void reallocate(GLsizeiptr size, const GLvoid* data) const override final {
            reallocate(size, data, GL_STATIC_DRAW);
        }

        void reallocate(GLsizeiptr size, const GLvoid* data, GLenum usage) const {
            if (isAllocated()) {
                bind();
                glBufferData(GL_ARRAY_BUFFER, size, data, usage);
                unbind();
            }
        }

        bool isCurrentlyInUse() const {
            return currentlyUsedID == id && isAllocated();
        }
    
    private:
        inline static GLuint currentlyUsedID = 0;
    };
}

#endif