#ifndef __MINIENGINE_MODEL__
#define __MINIENGINE_MODEL__

#include "mesh.hpp"
#include "../resources/material.hpp"
#include "../world/transform.hpp"

namespace MiniEngine {
    struct ModelSet {
        Mesh* mesh;
        Material* material;
    };

    class Model {
    public:
        struct Node {
            std::string name;
            Transform* transform;

            std::vector<ModelSet*> meshes;
            std::vector<Node*> nodes;
        };

        std::string_view getPath() const {
            return path;
        }

    private:
        std::string path;
        Node* root = nullptr;
    };
}

#endif