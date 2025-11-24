#pragma once

#ifndef __MINIENGINE_COMPONENT__
#define __MINIENGINE_COMPONENT__

#include "sceneobject.hpp"

namespace MiniEngine {
    namespace Components {

        class Component : public Base {
        public:
            SceneObject* sceneObject;

            Component(SceneObject* _sceneObject) : Base(_sceneObject->name), sceneObject(_sceneObject) {}

            MINIENGINE_DEFCLASSNAME(Component)
        };
    }
}

#endif
