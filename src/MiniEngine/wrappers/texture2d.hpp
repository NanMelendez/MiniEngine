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

        void allocate(i32 width, i32 height, i32 nChannels = 4, bool generateMipmap = true, const std::vector<std::pair<GLenum, GLint>>& params = {{GL_TEXTURE_WRAP_S, GL_REPEAT}, {GL_TEXTURE_WRAP_T, GL_REPEAT}, {GL_TEXTURE_MIN_FILTER, GL_LINEAR}, {GL_TEXTURE_MAG_FILTER, GL_LINEAR}}) {
            clear();
            clearImageData();

            glGenTextures(1, &id);

            imData = { NULL, nChannels, width, height };

            GLenum internalFormat;
            GLenum dataFormat;

            switch (imData.nChannels) {
            case 3:
                internalFormat = dataFormat = GL_RGB;
                break;
            case 4:
                internalFormat = dataFormat = GL_RGBA;
                break;
            default:
                internalFormat = dataFormat = GL_RED;
                break;
            }

            bind();
            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, imData.width, imData.height, 0, dataFormat, GL_UNSIGNED_BYTE, imData.data.get());
            if (generateMipmap)
                glGenerateMipmap(GL_TEXTURE_2D);

            for (const std::pair<GLenum, GLint>& param : params)
                setTextureParameter(param.first, param.second);
            
            unbind();
        }

        void upload(u8* data, bool generateMipmap = true, GLenum internalFormat = GL_RGBA, GLenum dataFormat = GL_RGBA) {
            imData.data.reset(data);
            
            bind();
            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, imData.width, imData.height, 0, dataFormat, GL_UNSIGNED_BYTE, imData.data.get());
            if (generateMipmap)
                glGenerateMipmap(GL_TEXTURE_2D);
            unbind();
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