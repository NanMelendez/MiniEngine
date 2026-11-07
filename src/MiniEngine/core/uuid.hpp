#ifndef __MINIENGINE_UUID_GENERATOR__
#define __MINIENGINE_UUID_GENERATOR__

#include "../pch.hpp"
#include "types.hpp"

namespace MiniEngine {
    class UUID {
    public:
        static UUID generate() {
            UUID uuid;
            
            uuid.high = genCtx.dist(genCtx.rng);
            uuid.low = genCtx.dist(genCtx.rng);

            uuid.high &= 0xFFFFFFFFFFFF0FFF;
            uuid.high |= 0x0000000000004000;

            uuid.low &= 0x3FFFFFFFFFFFFFFF;
            uuid.low |= 0x8000000000000000;

            return uuid;
        }

        static UUID fromString(const std::string& str) {
            std::string uuidStr = str;
            uuidStr.erase(std::remove(uuidStr.begin(), uuidStr.end(), '-'), uuidStr.end());

            if (uuidStr.length() != 32)
                return null();

            UUID uuid;

            std::from_chars_result resultHigh = std::from_chars(uuidStr.data(), uuidStr.data() + 16, uuid.high, 16);
            std::from_chars_result resultLow = std::from_chars(uuidStr.data() + 16, uuidStr.data() + 32, uuid.low, 16);

            if (resultHigh.ec != std::errc{} || resultHigh.ptr != uuidStr.data() + 16 || resultLow.ec != std::errc{} || resultLow.ptr != uuidStr.data() + 32)
                return null();

            return uuid;
        }

        bool operator==(const UUID& right) const {
            return high == right.high && low == right.low;
        }

        u64 hashKey() const {
            u64 hash = high;

            hash ^= low + 0x9e3779b97f4a7c15 + (hash << 6) + (hash >> 2);

            return hash;
        }

        std::pair<u64, u64> getComponents() const {
            return { high, low };
        }

        std::string toString() const {
            std::string result(36, '-');

            static const char hexDigits[] = "0123456789abcdef";

            for (i32 i = 0; i <= 7; i++)
                result[i] = hexDigits[(high >> (4 * (15 - i))) & 0xF];

            for (i32 i = 9; i <= 12; i++)
                result[i] = hexDigits[(high >> (4 * (16 - i))) & 0xF];

            for (i32 i = 14; i <= 17; i++)
                result[i] = hexDigits[(high >> (4 * (17 - i))) & 0xF];

            for (i32 i = 19; i <= 22; i++)
                result[i] = hexDigits[(low >> (4 * (34 - i))) & 0xF];

            for (i32 i = 24; i <= 34; i++)
                result[i] = hexDigits[(low >> (4 * (35 - i))) & 0xF];
            result[35] = hexDigits[low & 0xF];

            return result;
        }

        bool isValid() const {
            return high != 0 || low != 0;
        }

        bool isNull() const {
            return high == 0 && low == 0;
        }

        static UUID null() {
            return {};
        }
    
    private:
        struct GeneratorContext {
            std::mt19937_64 rng;
            std::uniform_int_distribution<u64> dist;

            GeneratorContext() {
                std::random_device rd;
                rng.seed(rd());
            }
        };

        inline static GeneratorContext genCtx {};

        u64 high = 0;
        u64 low = 0;
    };
}

namespace std {
    template<>
    struct hash<MiniEngine::UUID> {
        size_t operator()(const MiniEngine::UUID& uuid) const {
            return hash<MiniEngine::u64>()(uuid.hashKey());
        }
    };
}

#endif