#pragma once

#ifndef __MINIENGINE_SIMPLE_REFLECTION__
#define __MINIENGINE_SIMPLE_REFLECTION__

#include "../pch.hpp"
#include "types.hpp"

namespace MiniEngine {
    namespace Reflection {
        template<typename T>
        constexpr std::string_view typeName();

        template<>
        constexpr std::string_view typeName<void>() {
            return "void";
        }

        namespace Detail {
            template<typename T>
            constexpr std::string_view wrappedTypeName() {
#ifdef __clang__
                return __PRETTY_FUNCTION__;
#elif defined(__GNUC__)
                return __PRETTY_FUNCTION__;
#elif defined(_MSC_VER)
                return __FUNCSIG__;
#else
#error "Unsuported compiler."
#endif
            }

            constexpr std::size_t wrappedTypePrefixLength() {
                return wrappedTypeName<void>().find(typeName<void>());
            }

            constexpr std::size_t wrappedTypeSuffixLength() {
                return wrappedTypeName<void>().length() - wrappedTypePrefixLength() - typeName<void>().length();
            }
        }

        template<typename T>
        constexpr std::string_view typeName() {
            constexpr std::string_view wrappedName = Detail::wrappedTypeName<T>();
            constexpr std::size_t prefixLength = Detail::wrappedTypePrefixLength();
            constexpr std::size_t suffixLength = Detail::wrappedTypeSuffixLength();
            constexpr std::size_t typeNameLenght = wrappedName.length() - prefixLength - suffixLength;

            return wrappedName.substr(prefixLength, typeNameLenght);
        }

        constexpr u64 hash(std::string_view str) {

        }
    }
}

#endif
