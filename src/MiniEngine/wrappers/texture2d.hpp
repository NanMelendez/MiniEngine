#ifndef __MINIENGINE_2D_TEXTURE__
#define __MINIENGINE_2D_TEXTURE__

#include "../core/basetexture.hpp"
#include "../loaders/loaderbase.hpp"

namespace MiniEngine {
    class Texture2D : public Core::BaseTexture {
    public:
        template<typename T>
        friend class Loader;

        Texture2D() {}
        
        void bind(GLuint slot) const {
            useSlot(slot);
            bind();
        }

        void unbind(GLuint slot) const {
            useSlot(slot);
            unbind();
        }
        
        void setTextureParameter(GLenum name, const GLint param) const override final {
            glTexParameteri(GL_TEXTURE_2D, name, param);
        }

        bool isCurrentlyInUse() const {
            return currentlyUsedID == id && isAllocated();
        }
    
    private:
        inline static GLuint currentlyUsedID = 0;
        
        void bind() const override final {
            glBindTexture(GL_TEXTURE_2D, id);
            currentlyUsedID = id;
        }

        void unbind() const override final {
            glBindTexture(GL_TEXTURE_2D, 0);
            currentlyUsedID = 0;
        }
    };
}

#endif