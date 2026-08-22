#ifndef __MINIENGINE_BASE_TEXTURE__
#define __MINIENGINE_BASE_TEXTURE__

#include "basebindable.hpp"

namespace MiniEngine {
    namespace Core {
        struct ImageDataDeleter {
            void operator()(u8* data) const {
                if (data)
                    stbi_image_free(data);
            }
        };

        using ImageDataPtr = std::unique_ptr<u8[], ImageDataDeleter>;

        struct ImageData {
            ImageDataPtr data = nullptr;
            i32 nChannels;
            i32 width = 0, height = 0, depth = 0;
        };

        class BaseTexture : public BaseBindable {
        public:
            BaseTexture() {}

            BaseTexture(const BaseTexture&) = delete;
            BaseTexture& operator=(const BaseTexture&) = delete;

            BaseTexture(BaseTexture&&) noexcept = default;
            BaseTexture& operator=(BaseTexture&&) noexcept = default;

            ~BaseTexture() {
                clearImageData();
                clear();
            }

            virtual void setTextureParameter(GLenum name, const GLint param) const = 0;

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

            const ImageData& getImData() const {
                return imData;
            }

        protected:
            ImageData imData;

            void clearImageData() {
                if (imData.data) {
                    imData.data.reset();
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