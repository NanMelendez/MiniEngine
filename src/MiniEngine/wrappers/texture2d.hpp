#ifndef __MINIENGINE_2D_TEXTURE__
#define __MINIENGINE_2D_TEXTURE__

#include "../core/basetexture.hpp"

namespace MiniEngine {
    class Texture2D : public Core::BaseTexture {
    public:
        Texture2D() {}

        Texture2D(const Core::ImageData& imData, const std::vector<std::pair<GLenum, GLint>>& params = {{GL_TEXTURE_WRAP_S, GL_LINEAR}, {GL_TEXTURE_WRAP_T, GL_LINEAR}, {GL_TEXTURE_MIN_FILTER, GL_REPEAT}, {GL_TEXTURE_MAG_FILTER, GL_REPEAT}}) {
            load(imData, params);
        }

        void bind(GLuint slot) const {
            useSlot(slot);
            bind();
        }

        void unbind(GLuint slot) const {
            useSlot(slot);
            unbind();
        }

        void load(const Core::ImageData& imData, const std::vector<std::pair<GLenum, GLint>>& params = {{GL_TEXTURE_WRAP_S, GL_LINEAR}, {GL_TEXTURE_WRAP_T, GL_LINEAR}, {GL_TEXTURE_MIN_FILTER, GL_REPEAT}, {GL_TEXTURE_MAG_FILTER, GL_REPEAT}}) override final {
            clear();
            glGenTextures(1, &id);

            reload(imData, params);
        }

        void reload(const Core::ImageData& imData, const std::vector<std::pair<GLenum, GLint>>& params = {{GL_TEXTURE_WRAP_S, GL_LINEAR}, {GL_TEXTURE_WRAP_T, GL_LINEAR}, {GL_TEXTURE_MIN_FILTER, GL_REPEAT}, {GL_TEXTURE_MAG_FILTER, GL_REPEAT}}) override final {
            clearImageData();

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
            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, imData.width, imData.height, 0, dataFormat, GL_UNSIGNED_BYTE, imData.data);
            glGenerateMipmap(GL_TEXTURE_2D);
            
            for (const std::pair<GLenum, GLint>& param : params)
                setTextureParameter(param.first, param.second);
            
            unbind();
            
            this->imData = imData;
        }

        void setTextureParameter(GLenum name, const GLint param) const override final {
            glTexParameteri(GL_TEXTURE_2D, name, param);
        }

    private:
        void bind() const override final {
            glBindTexture(GL_TEXTURE_2D, id);
        }

        void unbind() const override final {
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    };

    Core::ImageData load2DImage(const std::string& path, bool flipVertically = false) {
        Core::ImageData imData;
        stbi_set_flip_vertically_on_load(flipVertically);
        imData.data = stbi_load(path.c_str(), &imData.width, &imData.height, &imData.nChannels, 0);

        if (imData.data == nullptr) {
            std::cerr << "Failed to load texture at \"" << path << "\"" << std::endl;

            // Create "error" texture placeholder
            imData.data = (u8*)malloc(sizeof(u8) * 3); // Because STB_Image uses malloc & free instead of the new & delete system
            imData.data[0] = 255;
            imData.data[1] = 0;
            imData.data[2] = 255;
            imData.width = imData.height = 1;
            imData.nChannels = 3;
        }

        return imData;
    }
}

#endif