#ifndef __MINIENGINE_SKYBOX_RENDERER__
#define __MINIENGINE_SKYBOX_RENDERER__

#include "../wrappers/vao.hpp"
#include "../wrappers/vbo.hpp"
#include "../resources/material.hpp"

namespace MiniEngine {
    class SkyboxRenderer {
    public:
        static void initialize() {
            vao.load(); vao.bind();
            vbo.load(sizeof(vertices), vertices); vbo.bind();
            vao.setAttribPointer(0, 3, 3 * sizeof(f32), (void*)0);
            vao.enableIndex(0);
            vao.unbind();
            vbo.unbind();
        }

        static void draw(const Material& material) {
            glDepthFunc(GL_LEQUAL);
            material.bind();
            vao.bind();
            glDrawArrays(GL_TRIANGLES, 0, 36);
            vao.unbind();
            material.unbind();
            glDepthFunc(GL_LESS);
        }

    private:
        inline static VAO vao;
        inline static VBO vbo;

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
    };
}

#endif