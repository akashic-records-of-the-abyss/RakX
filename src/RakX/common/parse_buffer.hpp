#pragma once
#include <array>
#include <limits>
#include <RakX/common/int.hpp>
#include <RakX/common/constants.hpp>
#include <bit>
#include <cassert>
#include <stdexcept>
#include <string>
#include <print>
#include <algorithm>

namespace RakX {
	class RawBuffer;


	namespace Detail {
		template<typename T>
		T parse_from_buffer(RawBuffer&, std::endian e) {
			assert(false);
			return T();
		}

		
		template<std::integral T>
		T parse_from_buffer(RawBuffer& other, std::endian e);

		template<typename T>
		void parse_to_buffer(const T&, RawBuffer&, std::endian e) {
			assert(false);
		}

		template<std::integral T>
		void parse_to_buffer(const T& val,RawBuffer& other, std::endian e);

	}

	template<typename T>
	concept Parsable = requires(T a, RawBuffer & b, std::endian e) {
		{ Detail::parse_from_buffer<T>(b, e) } -> std::same_as<T>;
	};

	template<typename T>
	concept Serializeable = requires(T a, RawBuffer& b, const T& c, std::endian e) {
		{ Detail::parse_to_buffer<T>(c, b, e) } -> std::same_as<void>;
	};

	class RawBuffer {
	public:
		RawBuffer() {
			this->m_buffer.fill(0xAF);
		};

		inline void* raw_data() noexcept { return static_cast<void*>(this->m_buffer.data());}

		template<std::integral T>
		inline T read_int() {
			std::array<u8, sizeof(T)> ret{ 0 }; 
			for (int x = 0; x < sizeof(T); x++) {
				ret[x] = this->read_byte();
			}
			return std::bit_cast<T>(ret);
		}

		template<std::integral T>
		inline void write_int(T val) {
			const auto bytes = std::bit_cast<std::array<u8, sizeof(T)>>(val);
			for (const u8 byte : bytes)
				this->write_byte(byte);
		}

		inline u8 read_byte() noexcept { return m_buffer[m_index++]; }
		inline void write_byte(u8 val) noexcept { 
			if (this->m_index == this->m_top)
				++this->m_top;
			m_buffer[m_index++] = val;
		}

		template<Parsable T>
		inline T read(std::endian endianess = std::endian::little) noexcept {
			return Detail::parse_from_buffer<T>(*this, endianess);
		}
		template<Serializeable T>
		inline RawBuffer&& write(const T& val, std::endian e = std::endian::little) noexcept {
			Detail::parse_to_buffer(val, *this, e);
			return std::move(*this);
		}

		inline usize size() const noexcept { return this->m_index; }
		inline void wipe() noexcept { 
			this->m_index = 0;
			std::fill_n(m_buffer.begin(), this->m_top, 0xAF);
			this->m_top = 0;
		}
		inline const u8* ptr() const noexcept { return (this->m_buffer.data() + m_index); };
		inline void skip(u16 count) noexcept { this->m_index += count; }
		inline void dump(u64 dumpTo) const noexcept {
			std::string dump{};
			for (u16 x = 0; x < dumpTo; x++) {
				dump += std::format("{:02x}", (u32)this->m_buffer[x]);
				if (x < dumpTo - 1) {
					dump += ", ";
				}
			}
			std::println("{}", dump);
		}

		inline usize max() const noexcept {
			return std::numeric_limits<u16>::max();
		}
		inline void top(usize top) noexcept {
			this->m_top = top;
		}
		inline void reset() noexcept {
			this->m_index = 0;
		}
	private:
		std::array<u8, std::numeric_limits<u16>::max()> m_buffer{0xAF};
		usize m_index{ 0 };
		usize m_top{ 0 };
	};


	namespace Detail {

		template<std::integral T>
		T parse_from_buffer(RawBuffer& other, std::endian e) {
			if (e != std::endian::native)
				return std::byteswap(other.read_int<T>());
			else
				return other.read_int<T>();
		}
		template<>
		inline std::string parse_from_buffer<std::string>(RawBuffer& other, std::endian e) {
			u16 leng = other.read<u16>(e);
			return std::string(
				(const char*)other.ptr(),
				leng
			);
		}
		template<>
		inline Magic parse_from_buffer<Magic>(RawBuffer& other, std::endian e) {
#ifndef NDEBUG
			constexpr auto expected = std::array<u8, 16> {
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
				0x78
			};
			for (int x = 0; x < 16; x++) {
				if (other.read_byte() != expected[x]) {
					throw std::runtime_error("Magic Incorrect");
				}
			}
#else	
			other.skip(16);
#endif
			return Magic{};
			
		}


		template<>
		inline bool parse_from_buffer(RawBuffer& other, std::endian e) {
			return (bool)other.read_byte();
		}

		template<std::integral T>
		inline void parse_to_buffer(const T& val, RawBuffer& other, std::endian e) {
			if (e != std::endian::native)
				other.write_int<T>(std::byteswap(val));
			else
				other.write_int<T>(val);
		}

		template<>
		inline void parse_to_buffer(const bool& val, RawBuffer& other, std::endian e) {
			other.write_byte((u8)val);
		}

		template<>
		inline void parse_to_buffer<std::string>(const std::string& val, RawBuffer& other, std::endian e) {
			const u16 size = (u16)val.size();
			other.write<u16>(size, std::endian::big);
			for (const char c : val) {
				other.write(c);
			}
		}
		template<>
		inline void parse_to_buffer<Magic>(const Magic& magic, RawBuffer& other, std::endian e) {
			for (const u8 byte : magic.MAGIC) {
				other.write_byte(byte);
			}

		}
	}
	
}