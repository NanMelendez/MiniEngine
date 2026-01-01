#pragma once

#ifndef __MINIENGINE_CORE_CONTAINER__
#define __MINIENGINE_CORE_CONTAINER__

#include "coreBaseObject.hpp"

namespace MiniEngine {
    namespace Core {
        class GLContainer : public GLObject {
        public:
            GLContainer() : GLObject() {}
            
            virtual void bind() const = 0;

            virtual void unbind() const = 0;
        };
    }
}

#endif
