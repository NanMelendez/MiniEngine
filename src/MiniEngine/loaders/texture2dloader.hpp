#ifndef __MINIENGINE_2D_TEXTURE_LOADER__
#define __MINIENGINE_2D_TEXTURE_LOADER__

#include "../wrappers/texture2d.hpp"

namespace MiniEngine {
    template<>
    class Loader<Texture2D> {
    public:
        static Texture2D load(std::string_view path, bool flipVertically = false, const std::vector<std::pair<GLenum, GLint>>& params = {{GL_TEXTURE_WRAP_S, GL_REPEAT}, {GL_TEXTURE_WRAP_T, GL_REPEAT}, {GL_TEXTURE_MIN_FILTER, GL_LINEAR}, {GL_TEXTURE_MAG_FILTER, GL_LINEAR}}) {
            Texture2D texture;

            glGenTextures(1, &texture.id);

            reload(texture, path, flipVertically, params);

            return texture;
        }

        static void reload(Texture2D& texture, std::string_view path, bool flipVertically = false, const std::vector<std::pair<GLenum, GLint>>& params = {{GL_TEXTURE_WRAP_S, GL_REPEAT}, {GL_TEXTURE_WRAP_T, GL_REPEAT}, {GL_TEXTURE_MIN_FILTER, GL_LINEAR}, {GL_TEXTURE_MAG_FILTER, GL_LINEAR}}) {
            texture.clearImageData();

            texture.imData = fetchImageData(path, flipVertically);

            GLenum internalFormat;
            GLenum dataFormat;

            switch (texture.imData.nChannels) {
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

            texture.bind();
            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, texture.imData.width, texture.imData.height, 0, dataFormat, GL_UNSIGNED_BYTE, texture.imData.data.get());
            glGenerateMipmap(GL_TEXTURE_2D);
            
            for (const std::pair<GLenum, GLint>& param : params)
                texture.setTextureParameter(param.first, param.second);
            
            texture.unbind();
        }
        
        private:
            static Core::ImageData fetchImageData(std::string_view path, bool flipVertically) {
                Core::ImageData imData;
                stbi_set_flip_vertically_on_load(flipVertically);
                imData.data.reset(stbi_load(std::string(path).c_str(), &imData.width, &imData.height, &imData.nChannels, 0));

                if (imData.data == nullptr) {
                    std::cerr << "Failed to load texture at \"" << path << "\"" << std::endl;

                    // Create "error" texture placeholder
                    imData.data.reset((u8*)malloc(sizeof(u8) * 3)); // Because STB_Image uses malloc & free instead of the new & delete system
                    imData.data[0] = 255;
                    imData.data[1] = 0;
                    imData.data[2] = 255;
                    imData.width = imData.height = 1;
                    imData.nChannels = 3;
                }

                return imData;
            }
    };
}

#endif