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
            Transform* transform;

            std::vector<Component*> components;
            
            std::string tag = "None";
            i32 layer;

            Scene* scene = nullptr;

            SceneObject(const std::string& _name, Transform* _transform, const std::vector<Component*>& _components = {}) : Base(_name), transform(_transform), components(_components) {}

            MINIENGINE_DEFCLASSNAME(SceneObject)
        };
    }
}

#endif
