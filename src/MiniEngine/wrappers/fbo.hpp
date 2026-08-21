#ifndef __MINIENGINE_FRAMEBUFFER_OBJECT__
#define __MINIENGINE_FRAMEBUFFER_OBJECT__

#include "../core/basebindable.hpp"
#include "../mesh/mesh.hpp" 
#include "texture2d.hpp"
#include "rbo.hpp"
#include "../resources/material.hpp"

namespace MiniEngine {
    class FBO : public Core::BaseBindable {
    public:
        FBO() {}

        FBO(i32 width, i32 height, const MeshData& target = FBO::defaultTarget()) {
            load(width, height, target);
        }

        void load(i32 width, i32 height, const MeshData& target = FBO::defaultTarget()) {
            clear();

            resolution = glm::uvec2(width, height);
            this->target = Mesh(target);

            glGenFramebuffers(1, &id);
            bind();

            colorBuffer.allocate(width, height, 4, false);
            colorBuffer.bind(0);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer.getID(), 0);

            renderBuffer.load(width, height);
            renderBuffer.bind();
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBuffer.getID());

            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                std::cerr << "ERROR::FRAMEBUFFER::Framebuffer is not complete!" << std::endl;
            
            unbind();
            renderBuffer.unbind();
        }

        void resize(i32 width, i32 height) {
            bind();

            resolution = glm::uvec2(width, height);

            colorBuffer.allocate(width, height, 4, false);
            colorBuffer.bind(0);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer.getID(), 0);

            renderBuffer.reload(width, height);
            renderBuffer.bind();
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBuffer.getID());

            unbind();
            renderBuffer.unbind();
        }

        void bind() const override final {
            bind(GL_FRAMEBUFFER);
        }

        void bind(GLenum fbTarget) const {
            glBindFramebuffer(fbTarget, id);
            glViewport(0, 0, resolution.x, resolution.y);
        }

        void unbind() const override final {
            unbind(GL_FRAMEBUFFER);
        }

        void unbind(GLenum fbTarget) const {
            glBindFramebuffer(fbTarget, 0);
        }

        void clear() override final {
            if (isAllocated()) {
                glDeleteFramebuffers(1, &id);
                id = 0;
                colorBuffer.clear();
                renderBuffer.clear();
            }
        }

        void render(Material& material) {
            glDisable(GL_DEPTH_TEST);
            
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            material.set<Texture2D*>("targetTexture", &colorBuffer);

            material.bind();
            target.getVAO().bind();
            glDrawElements(GL_TRIANGLES, target.getEBO().getCount(), GL_UNSIGNED_INT, 0);

            material.unbind();

            glEnable(GL_DEPTH_TEST);
        }

        glm::uvec2 getResolution() const {
            return resolution;
        }

    private:
        glm::uvec2 resolution;
        Mesh target;

        Texture2D colorBuffer;
        RBO renderBuffer;

        static MeshData defaultTarget() {
            return MeshData(
                {
                    { {  1.0f,  1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } },
                    { { -1.0f,  1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } },
                    { { -1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
                    { {  1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } }
                },
                {
                    0, 1, 3,
                    1, 2, 3
                }
            );
        }
    };
}

#endif