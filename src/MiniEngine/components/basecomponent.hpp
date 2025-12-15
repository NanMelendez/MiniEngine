#pragma once

#ifndef __MINIENGINE_CBA_STARTING_POINT__
#define __MINIENGINE_CBA_STARTING_POINT__

#include "../pch.hpp"
#include "../extras/types.hpp"

#define MINIENGINE_DEFCLASSNAME(c)\
protected:\
    std::string className() const override {\
        return #c;\
    }

namespace MiniEngine {
    namespace Components {
        enum class HideFlags : u8 {
            NONE     = 0,
            NOVIEW   = 1 << 1,
            NORENDER = 1 << 2,
            NOEDIT   = 1 << 3
        };

        HideFlags operator&(HideFlags a, HideFlags b) {
            return static_cast<HideFlags>(static_cast<u8>(a) & static_cast<u8>(b));
        }

        HideFlags operator|(HideFlags a, HideFlags b) {
            return static_cast<HideFlags>(static_cast<u8>(a) | static_cast<u8>(b));
        }

        HideFlags operator^(HideFlags a, HideFlags b) {
            return static_cast<HideFlags>(static_cast<u8>(a) ^ static_cast<u8>(b));
        }

        HideFlags operator!(HideFlags a) {
            return static_cast<HideFlags>(!static_cast<u8>(a));
        }

        HideFlags& operator&=(HideFlags& a, HideFlags b) {
            a = a & b;
            return a;
        }

        HideFlags& operator|=(HideFlags& a, HideFlags b) {
            a = a | b;
            return a;
        }

        HideFlags& operator^=(HideFlags& a, HideFlags b) {
            a = a ^ b;
            return a;
        }

        class Base {
        public:
            Base(const std::string& _name) {}

            virtual ~Base() = default;

            std::string name;
            HideFlags hideFlags = HideFlags::NONE;

            bool operator==(const Base& other) {
                return this == &other;
            }

            bool operator!=(const Base& other) {
                return !(*this == other);
            }

            operator bool() {
                return this != nullptr;
            }
            
            std::string toString() const {
                return name + " (" + className() + ')';
            }

            static void destroy(std::shared_ptr<Base> object) {
                // ...
            }

        protected:
            virtual std::string className() const {
                return "Base";
            };
        };
    }
}

#endif
