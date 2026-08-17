#ifndef __MINIENGINE_VERTEX_ARRAY_OBJECT__
#define __MINIENGINE_VERTEX_ARRAY_OBJECT__

#include "../core/basebindable.hpp"

namespace MiniEngine {
    class VAO final : public Core::BaseBindable {
    public:
        VAO(bool loadOnInit = false) {
            if (loadOnInit)
                load();
        }

        void bind() const override final {
            glBindVertexArray(id);
            currentlyUsedID = id;
        }

        void unbind() const override final {
            glBindVertexArray(0);
            currentlyUsedID = 0;
        }

        void load() {
            clear();
            glGenVertexArrays(1, &id);
        }

        void clear() override final {
            if (isAllocated()) {
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

        bool isCurrentlyInUse() const {
            return currentlyUsedID == id && isAllocated();
        }
    
    private:
        inline static GLuint currentlyUsedID = 0;
    };
}

#endif