#ifndef __MINIENGINE_MESH__
#define __MINIENGINE_MESH__

#include "../wrappers/vao.hpp"
#include "../wrappers/vbo.hpp"
#include "../wrappers/ebo.hpp"

namespace MiniEngine {
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 uv;
    };

    struct MeshData {
        std::vector<Vertex> vertices;
        std::vector<u32> indices;
    };

    class Mesh {
    public:
        MeshData data;

        Mesh() {}

        Mesh(const std::vector<Vertex>& vertices, const std::vector<u32>& indices) : data({vertices, indices}) {
            setup();
        }

        Mesh(const MeshData& data) : data(data) {
            setup();
        }

        const VAO& getVAO() const {
            return vao;
        }

        const EBO& getEBO() const {
            return ebo;
        }

    private:
        VAO vao;
        VBO vbo;
        EBO ebo;

        void setup() {
            vao.load(); vao.bind();
            vbo.load(data.vertices.size() * sizeof(Vertex), data.vertices.data()); vbo.bind();
            ebo.load(data.indices.size() * sizeof(u32), data.indices.data()); ebo.bind();

            vao.setAttribPointer(0, 3, sizeof(Vertex), (void*)offsetof(Vertex, position));
            vao.enableIndex(0);
            vao.setAttribPointer(1, 3, sizeof(Vertex), (void*)offsetof(Vertex, normal));
            vao.enableIndex(1);
            vao.setAttribPointer(2, 2, sizeof(Vertex), (void*)offsetof(Vertex, uv));
            vao.enableIndex(2);

            vao.unbind();
            vbo.unbind();
            ebo.unbind();
        }
    };
}

#endif