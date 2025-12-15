#pragma once

#ifndef __MINIENGINE_SCENE_OBJECT__
#define __MINIENGINE_SCENE_OBJECT__

#include "basecomponent.hpp"

namespace MiniEngine {
    namespace Components {
        class Transform;
        class Component;
        class Scene;

        class SceneObject : public Base {
        public:
            std::shared_ptr<Transform> transform;
            
            std::string tag = "None";
            i32 layer;

            std::shared_ptr<Scene> scene = nullptr;

            SceneObject(const std::string& _name, Transform* _transform, const std::vector<Component*>& _components = {}) : Base(_name), transform(_transform), components(_components) {}

            template<typename T>
            void addComponent() {

            }

            MINIENGINE_DEFCLASSNAME(SceneObject)

        private:
            std::vector<std::shared_ptr<Component>> components;
        };
    }
}

#endif
