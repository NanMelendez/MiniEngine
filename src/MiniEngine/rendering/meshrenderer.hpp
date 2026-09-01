#ifndef __MINIENGINE_MESH_RENDERER__
#define __MINIENGINE_MESH_RENDERER__

#include "../mesh/mesh.hpp"
#include "../resources/material.hpp"
#include "../world/transform.hpp"

namespace MiniEngine {
    class MeshRenderer {
    public:
        static void draw(const Mesh& mesh, const Material& mat, const Transform& transform) {
            glm::mat4 M = transform.model();

            mat.bind();
            mat.getShader()->setUniform("M", M);
            mat.getShader()->setUniform("mN", glm::mat3(glm::transpose(glm::inverse(M))));

            mesh.getVAO().bind();
            glDrawElements(GL_TRIANGLES, mesh.getEBO().getCount(), GL_UNSIGNED_INT, 0);
            mesh.getVAO().unbind();

            mat.unbind();
        }
    };
}

#endif