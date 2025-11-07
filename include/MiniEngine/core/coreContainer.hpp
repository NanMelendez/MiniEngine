#pragma once

#ifndef __MINIENGINE_CORE_CONTAINER__
#define __MINIENGINE_CORE_CONTAINER__

#include "coreBaseObject.hpp"

namespace MiniEngine {
    namespace Core {
        class Container : public Object {
        public:
            Container() : Object() {}
            
            virtual void bind() const = 0;

            virtual void unbind() const = 0;
        };
    }
}

#endif
