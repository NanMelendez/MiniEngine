#ifndef __MINIENGINE_PREFAB_MESHES__
#define __MINIENGINE_PREFAB_MESHES__

#include "../world/transform.hpp"
#include "mesh.hpp"

namespace MiniEngine {
    class Prefabs {
    public:
        static Mesh plane(const Transform& transform, u32 verticesX = 2u, u32 verticesY = 2u) {
            std::vector<Vertex> vertices;
            std::vector<u32> indices;

            verticesX = glm::max(verticesX, 2u);
            verticesY = glm::max(verticesY, 2u);

            meshGrid(vertices, indices, transform.model(), verticesX, verticesY);

            return Mesh(vertices, indices);
        }

        static Mesh cube(const Transform& transform, u32 verticesX = 2u, u32 verticesY = 2u, u32 verticesZ = 2u) {
            std::vector<Vertex> vertices;
            std::vector<u32> indices;

            verticesX = glm::max(verticesX, 2u);
            verticesY = glm::max(verticesY, 2u);
            verticesZ = glm::max(verticesZ, 2u);

            glm::mat4 model = transform.model();
            
            // Top
            meshGrid(vertices, indices, model * Transform(glm::vec3( 0.0f,  0.5f,  0.0f), glm::radians(glm::vec3( 0.0f,   0.0f,    0.0f)), glm::vec3(1.0f)).model(), verticesX, verticesZ);
            // Bottom
            meshGrid(vertices, indices, model * Transform(glm::vec3( 0.0f, -0.5f,  0.0f), glm::radians(glm::vec3( 0.0f,   0.0f, -180.0f)), glm::vec3(1.0f)).model(), verticesX, verticesZ);
            // Left
            meshGrid(vertices, indices, model * Transform(glm::vec3(-0.5f,  0.0f,  0.0f), glm::radians(glm::vec3(90.0f, -90.0f,    0.0f)), glm::vec3(1.0f)).model(), verticesZ, verticesY);
            // Right
            meshGrid(vertices, indices, model * Transform(glm::vec3( 0.5f,  0.0f,  0.0f), glm::radians(glm::vec3(90.0f,  90.0f,    0.0f)), glm::vec3(1.0f)).model(), verticesZ, verticesY);
            // Front
            meshGrid(vertices, indices, model * Transform(glm::vec3( 0.0f,  0.0f,  0.5f), glm::radians(glm::vec3(90.0f,   0.0f,    0.0f)), glm::vec3(1.0f)).model(), verticesX, verticesY);
            // Back
            meshGrid(vertices, indices, model * Transform(glm::vec3( 0.0f,  0.0f, -0.5f), glm::radians(glm::vec3(90.0f, 180.0f,    0.0f)), glm::vec3(1.0f)).model(), verticesX, verticesY);

            return Mesh(vertices, indices);
        }

    private:
        static void meshGrid(std::vector<Vertex>& vertices, std::vector<u32>& indices, const glm::mat4& model, u32 resolutionX, u32 resolutionY) {
            f32 stepX = 1.0f / (resolutionX - 1);
            f32 stepY = 1.0f / (resolutionY - 1);

            u32 baseIndex = vertices.size();

            glm::vec3 normal = glm::normalize(glm::transpose(glm::inverse(glm::mat3(model))) * glm::vec3(0.0f, 1.0f, 0.0f));

            for (u32 i = 0; i < resolutionY; i++) {
                for (u32 j = 0; j < resolutionX; j++) {
                    f32 factorX = stepX * j;
                    f32 factorY = stepY * i;

                    vertices.emplace_back(
                        model * glm::vec4(glm::mix(glm::vec3(-0.5f, 0.0f, 0.5f), glm::vec3(0.5f, 0.0f, -0.5f), glm::vec3(factorX, 0.0f, factorY)), 1.0f),
                        normal,
                        glm::mix(glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(factorX, factorY))
                    );
                }
            }

            for (u32 i = 0; i < resolutionY - 1; i++) {
                for (u32 j = 0; j < resolutionX - 1; j++) {
                    u32 v0 = baseIndex + i * resolutionX + j;
                    u32 v1 = v0 + 1;
                    u32 v2 = v0 + resolutionX;
                    u32 v3 = v2 + 1;

                    indices.emplace_back(v0);
                    indices.emplace_back(v1);
                    indices.emplace_back(v3);

                    indices.emplace_back(v0);
                    indices.emplace_back(v3);
                    indices.emplace_back(v2);
                }
            }
        }
    };
}

#endif