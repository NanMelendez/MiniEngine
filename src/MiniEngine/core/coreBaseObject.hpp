#pragma once

#ifndef __MINIENGINE_CORE_BASE_OBJECT__
#define __MINIENGINE_CORE_BASE_OBJECT__

#include "../pch.hpp"
#include "../extras/types.hpp"

namespace MiniEngine {
    namespace Core {
        class Object {
        public:
            Object() {}

            u32 getID() const { return id; }

            virtual void clear() = 0;
        protected:
            u32 id = 0;
        };
    }
}

#endif
