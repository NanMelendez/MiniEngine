#ifndef __MINIENGINE_RENDER_BUFFER_OBJECT__
#define __MINIENGINE_RENDER_BUFFER_OBJECT__

#include "../core/basebindable.hpp"

namespace MiniEngine {
    class RBO : public Core::BaseBindable {
    public:
        RBO() {}

        RBO(i32 width, i32 height, i32 samples = 0, GLenum internalFormat = GL_DEPTH24_STENCIL8) {
            load(width, height, samples, internalFormat);
        }

        void load(i32 width, i32 height, i32 samples = 0, GLenum internalFormat = GL_DEPTH24_STENCIL8) {
            clear();
            glGenRenderbuffers(1, &id);
            reload(width, height, samples, internalFormat);
        }

        void reload(i32 width, i32 height, i32 samples = 0, GLenum internalFormat = GL_DEPTH24_STENCIL8) {
            bind();
            if (samples > 0)
                glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, internalFormat, width, height);
            else
                glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width, height);
            unbind();
        }

        void bind() const override final {
            glBindRenderbuffer(GL_RENDERBUFFER, id);
        }

        void unbind() const override final {
            glBindRenderbuffer(GL_RENDERBUFFER, 0);
        }

        void clear() override final {
            if (isAllocated()) {
                glDeleteRenderbuffers(1, &id);
                id = 0;
            }
        }
    };
}

#endif