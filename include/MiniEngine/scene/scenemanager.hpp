#pragma once

#ifndef __MINIENGINE_SCENE_MANAGER__
#define __MINIENGINE_SCENE_MANAGER__

#include "../pch.hpp"
#include "scene.hpp"

namespace MiniEngine {
    class SceneManager {
    public:
        static SceneManager& Instance() {
            return *instance;
        }

        std::vector<std::string> listScenes() {
            std::vector<std::string> names;

            for (auto it = scenes.begin(); it != scenes.end(); it++)
                names.push_back(it->first);
            
            return names;
        }

        std::shared_ptr<Scene> getCurrentScene(const std::string& name) {
            auto temp = scenes.find(name);
            
            return temp != scenes.end() ? temp->second : nullptr;
        }

        void loadScene(const std::string& name) {
            auto temp = scenes.find(name);
        }

    private:
        Scene* current = nullptr;
        std::unordered_map<std::string, std::shared_ptr<Scene>> scenes;

        static SceneManager* instance;
    };
}

#endif
