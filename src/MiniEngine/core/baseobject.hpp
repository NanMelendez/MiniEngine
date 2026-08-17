#ifndef __MINIENGINE_CORE_OBJECT__
#define __MINIENGINE_CORE_OBJECT__

#include "../pch.hpp"
#include "types.hpp"

namespace MiniEngine {
    namespace Core {
        class BaseObject {
        public:
            BaseObject() {}

            BaseObject(const BaseObject&) = delete;
            BaseObject& operator=(const BaseObject&) = delete;

            BaseObject(BaseObject&& other) noexcept : id(other.id) {
                other.id = 0;
            }

            virtual ~BaseObject() = default;

            BaseObject& operator=(BaseObject&& other) noexcept {
                if (this != &other) {
                    id = other.id;
                    other.id = 0;
                }
                return *this;
            }

            GLuint getID() const { return id; }

            virtual void clear() = 0;

            bool isAllocated() const { return id != 0; }

        protected:
            GLuint id = 0;
        };
    }
}

#endif