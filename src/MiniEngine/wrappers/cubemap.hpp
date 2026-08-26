#ifndef __MINIENGINE_CUBEMAP__
#define __MINIENGINE_CUBEMAP__

#include "texture2d.hpp"
#include "vao.hpp"
#include "vbo.hpp"
// #include "../resources/material.hpp"

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

        Cubemap() {
            meshLoad();
        }

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
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + static_cast<i32>(face.first), 0, GL_RGB, imData.width, imData.height, 0, GL_RGB, GL_UNSIGNED_BYTE, imData.data.get());
        }

        void draw(const Material& material, i32 slot = 0) const {
            glDepthFunc(GL_LEQUAL);
            material.bind();
            vao.bind();
            glDrawArrays(GL_TRIANGLES, 0, 36);
            vao.unbind();
            material.unbind();
            glDepthFunc(GL_LESS);
        }

    private:
        std::unordered_map<CubemapFace, Texture2D*> faces;

        VAO vao;
        VBO vbo;

        inline static GLuint currentlyUsedID = 0;

        inline static f32 vertices[108] = {
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
            
            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f,  1.0f
        };

        void meshLoad() {
            vao.load(); vao.bind();
            vbo.load(sizeof(vertices), vertices); vbo.bind();
            vao.setAttribPointer(0, 3, 3 * sizeof(f32), (void*)0);
            vao.enableIndex(0);
            vao.unbind();
            vbo.unbind();
        }

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