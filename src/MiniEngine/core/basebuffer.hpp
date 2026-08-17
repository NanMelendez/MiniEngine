#ifndef __MINIENGINE_BASE_BUFFER__
#define __MINIENGINE_BASE_BUFFER__

#include "basebindable.hpp"

namespace MiniEngine {
    namespace Core {
        class BaseBuffer: public BaseBindable {
        public:
            BaseBuffer() {}

            virtual void load(GLsizeiptr size, const GLvoid* data) = 0;

            void clear() override final {
                if (isAllocated()) {
                    glDeleteBuffers(1, &id);
                    id = 0;
                }
            }

            virtual void update(GLsizeiptr size, const GLvoid* data, GLintptr offset) const = 0;

            virtual void reallocate(GLsizeiptr size, const GLvoid* data) const = 0;
        };
    }
}

#endif