#pragma once

#ifndef __MINIENGINE_SCENE__
#define __MINIENGINE_SCENE__

#include "../pch.hpp"
#include "../extras/types.hpp"

namespace MiniEngine {
    class Scene {
    public:
        std::string name;
        
        u32 getID() const { return id; }

    private:
        u32 id;
    };
}

#endif
