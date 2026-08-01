#ifndef __MINIENGINE_BASE_BINDABLE__
#define __MINIENGINE_BASE_BINDABLE__

#include "baseobject.hpp"

namespace MiniEngine {
    namespace Core {
        class BaseBindable : public BaseObject {
        public:
            BaseBindable() {}

            virtual void bind() const = 0;

            virtual void unbind() const = 0;
        };
    }
}

#endif