#ifndef __MINIENGINE_BASE_TEXTURE__
#define __MINIENGINE_BASE_TEXTURE__

#include "basebindable.hpp"

namespace MiniEngine {
    namespace Core {
        struct ImageData {
            u8* data = nullptr;
            i32 nChannels;
            i32 width = 0, height = 0, depth = 0;
        };

        class BaseTexture : public BaseBindable {
        public:
            BaseTexture() {}

            ~BaseTexture() {
                clearImageData();
            }

            virtual void setTextureParameter(GLenum name, const GLint param) const = 0;

            virtual void load(const ImageData& imData, const std::vector<std::pair<GLenum, GLint>>& params) = 0;

            virtual void reload(const ImageData& imData, const std::vector<std::pair<GLenum, GLint>>& params) = 0;

            void clear() override final {
                if (isAllocated()) {
                    glDeleteTextures(1, &id);
                    id = 0;
                }
            }

            static i32 maxTextureSlots() {
                i32 maxSlots = 0;
                glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxSlots);
                return maxSlots;
            }

        protected:
            ImageData imData;

            void clearImageData() {
                if (imData.data) {
                    free(imData.data);
                    imData.data = nullptr;
                }
            }

            void useSlot(GLuint idx) const {
                glActiveTexture(GL_TEXTURE0 + idx);
            }
        };
    }
}

#endif