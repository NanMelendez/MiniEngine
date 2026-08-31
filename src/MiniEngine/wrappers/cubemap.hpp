#ifndef __MINIENGINE_CUBEMAP__
#define __MINIENGINE_CUBEMAP__

#include "texture2d.hpp"
#include "vao.hpp"
#include "vbo.hpp"

namespace MiniEngine {
    class Material;

    enum class CubemapFace : i32 {
        X_POSITIVE = 0,
        X_NEGATIVE = 1,
        Y_POSITIVE = 2,
        Y_NEGATIVE = 3,
        Z_POSITIVE = 4,
        Z_NEGATIVE = 5
    };

    class Cubemap : public Core::BaseTexture {
    public:
        template<typename T>
        friend class Loader;
        
        void bind(GLuint slot) const {
            useSlot(slot);
            bind();
        }

        void unbind(GLuint slot) const {
            useSlot(slot);
            unbind();
        }

        bool isCurrentlyInUse() const {
            return currentlyUsedID == id && isAllocated();
        }

        void setTextureParameter(GLenum name, const GLint param) const {
            glTexParameteri(GL_TEXTURE_CUBE_MAP, name, param);
        }

        void setFace(const std::pair<CubemapFace, Texture2D*>& face) {
            faces[face.first] = face.second;
            const Core::ImageData& imData = face.second->getImData();

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

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + static_cast<i32>(face.first), 0, internalFormat, imData.width, imData.height, 0, dataFormat, GL_UNSIGNED_BYTE, imData.data.get());
        }

    private:
        std::unordered_map<CubemapFace, Texture2D*> faces;

        inline static GLuint currentlyUsedID = 0;

        void bind() const override final {
            glBindTexture(GL_TEXTURE_CUBE_MAP, id);
            currentlyUsedID = id;
        }

        void unbind() const override final {
            glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
            currentlyUsedID = 0;
        }
    };
}

#endif