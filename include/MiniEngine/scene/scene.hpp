#pragma once

#ifndef __MINIENIGNE_SCENE__
#define __MINIENIGNE_SCENE__

#include "../pch.hpp"
#include "../extras/types.hpp"

namespace MiniEngine {
    class Scene {
    public:
        u32 getID() const { return id; }

    private:
        u32 id;
    };
}

#endif
