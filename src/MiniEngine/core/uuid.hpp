#ifndef __MINIENGINE_UUID_GENERATOR__
#define __MINIENGINE_UUID_GENERATOR__

#include "../pch.hpp"
#include "types.hpp"

namespace MiniEngine {
    class UUID {
    public:
        static UUID generate() {
            UUID uuid;

            std::random_device rd;
            std::mt19937_64 rng(rd());
            std::uniform_int_distribution<u64> dist;

            uuid.high = dist(rng);
            uuid.low = dist(rng);

            uuid.high &= 0xFFFFFFFFFFFF0FFF;
            uuid.high |= 0x0000000000004000;

            uuid.low &= 0x3FFFFFFFFFFFFFFF;
            uuid.low |= 0x8000000000000000;

            return uuid;
        }

        bool operator==(const UUID& right) const {
            return high == right.high && low == right.low;
        }

        operator u64() const {
            return high ^ low;
        }

        std::pair<u64, u64> getComponents() {
            return { high, low };
        }

    private:
        u64 high;
        u64 low;
    };
}

namespace std {
    template<>
    struct hash<MiniEngine::UUID> {
        size_t operator()(const MiniEngine::UUID& uuid) const {
            return hash<MiniEngine::u64>()((MiniEngine::u64)uuid);
        }
    };
}

#endif