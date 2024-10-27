#pragma once
#include <RakX/common/int.hpp>
#include <array>
#include <bit>
namespace RakX {
    struct Magic {
        const std::array<u8, 16> MAGIC = {
        0x00,
        0xff,
        0xff,
        0x00,
        0xfe,
        0xfe,
        0xfe,
        0xfe,
        0xfd,
        0xfd,
        0xfd,
        0xfd,
        0x12,
        0x34,
        0x56,
        0x78,
        };
        Magic() = default;
        Magic(const Magic& other) {}
        Magic operator=(const Magic& other) { return *this; }
    };
    
    template<typename T>
    concept NotAReference = !std::is_reference_v<T>;
    constexpr usize UDP_HEADER_SIZE = 28;

    template<std::endian e, std::integral T>
    inline constexpr T as_endian(T val) {
        if constexpr (e != std::endian::native) {
            return std::byteswap(val);
        }
        else {
            return val;
        }
    }

}

#ifndef NDEBUG
#define rel_noexcept 
#else
#define rel_noexcept noexcept
#endif