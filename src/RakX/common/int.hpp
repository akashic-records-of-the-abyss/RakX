#pragma once
#include <cstddef>
#include <concepts>


namespace RakX {
	using u8 = unsigned char;
	using u16 = unsigned short;
	using u32 = unsigned int;
	using u64 = unsigned long long;

	using i8 = signed char;
	using i16 = signed short;
	using i32 = signed int;
	using i64 = signed long long;

	using f32 = float;
	using f64 = double;

	// Size types
	using usize = std::size_t;
	using isize = std::ptrdiff_t;

    class u24 {
    public:
        u24(u8 value) : self_value(static_cast<u32>(value)& AND_VAL) {}
        u24(u16 value) : self_value(static_cast<u32>(value)& AND_VAL) {}
        u24(u32 value) : self_value(value& AND_VAL) {}
        u24(u64 value) : self_value(static_cast<u32>(value)& AND_VAL) {}

        u24(i8 value) : self_value(static_cast<u32>(value)& AND_VAL) {}
        u24(i16 value) : self_value(static_cast<u32>(value)& AND_VAL) {}
        u24(i32 value) : self_value(static_cast<u32>(value)& AND_VAL) {}
        u24(i64 value) : self_value(static_cast<u32>(value)& AND_VAL) {}

        u24(f32 value) : self_value(static_cast<u32>(value)& AND_VAL) {}
        u24(f64 value) : self_value(static_cast<u32>(value)& AND_VAL) {}

        operator u8() const noexcept { return static_cast<u8>(self_value); }
        operator u16() const noexcept { return static_cast<u16>(self_value); }
        operator u32() const noexcept { return self_value; }
        operator u64() const noexcept { return static_cast<u64>(self_value); }

        operator i8() const noexcept { return static_cast<i8>(self_value); }
        operator i16() const noexcept { return static_cast<i16>(self_value); }
        operator i32() const noexcept { return static_cast<i32>(self_value); }
        operator i64() const noexcept { return static_cast<i64>(self_value); }

        operator f32() const noexcept { return static_cast<f32>(self_value); }
        operator f64() const noexcept { return static_cast<f64>(self_value); }

        u24 operator+(u24 other) const noexcept { return u24((self_value + other.self_value) & AND_VAL); }
        u24 operator-(u24 other) const noexcept { return u24((self_value - other.self_value) & AND_VAL); }
        u24 operator*(u24 other) const noexcept { return u24((self_value * other.self_value) & AND_VAL); }
        u24 operator/(u24 other) const noexcept { return u24(other.self_value != 0 ? (self_value / other.self_value) & AND_VAL : 0); }
        u24 operator%(u24 other) const noexcept { return u24(other.self_value != 0 ? (self_value % other.self_value) & AND_VAL : 0); }

        u24& operator+=(u24 other) noexcept { return *this = *this + other; }
        u24& operator-=(u24 other) noexcept { return *this = *this - other; }
        u24& operator*=(u24 other) noexcept { return *this = *this * other; }
        u24& operator/=(u24 other) noexcept { return *this = *this / other; }
        u24& operator%=(u24 other) noexcept { return *this = *this % other; }

        template<std::integral T>
        u24 operator+(T other) const noexcept { return u24((self_value + static_cast<u32>(other)) & AND_VAL); }
        template<std::integral T>
        u24 operator-(T other) const noexcept { return u24((self_value - static_cast<u32>(other)) & AND_VAL); }
        template<std::integral T>
        u24 operator*(T other) const noexcept { return u24((self_value * static_cast<u32>(other)) & AND_VAL); }
        template<std::integral T>
        u24 operator/(T other) const noexcept { return u24(other != 0 ? (self_value / static_cast<u32>(other)) & AND_VAL : 0); }
        template<std::integral T>
        u24 operator%(T other) const noexcept { return u24(other != 0 ? (self_value % static_cast<u32>(other)) & AND_VAL : 0); }

        template<std::integral T>
        u24& operator+=(T other) noexcept { return *this = *this + other; }
        template<std::integral T>
        u24& operator-=(T other) noexcept { return *this = *this - other; }
        template<std::integral T>
        u24& operator*=(T other) noexcept { return *this = *this * other; }
        template<std::integral T>
        u24& operator/=(T other) noexcept { return *this = *this / other; }
        template<std::integral T>
        u24& operator%=(T other) noexcept { return *this = *this % other; }

        bool operator==(const u24& other) const noexcept { return self_value == other.self_value; }
        bool operator!=(const u24& other) const noexcept { return self_value != other.self_value; }
        bool operator<(const u24& other) const noexcept { return self_value < other.self_value; }
        bool operator<=(const u24& other) const noexcept { return self_value <= other.self_value; }
        bool operator>(const u24& other) const noexcept { return self_value > other.self_value; }
        bool operator>=(const u24& other) const noexcept { return self_value >= other.self_value; }

    private:
        static constexpr u32 AND_VAL = 0xFFFFFF;
        u32 self_value{ 0 };
    };


}