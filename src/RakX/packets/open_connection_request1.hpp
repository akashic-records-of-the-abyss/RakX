#pragma once
#include <RakX/common/int.hpp>
#include <RakX/common/constants.hpp>
#include <RakX/common/parse_buffer.hpp>

namespace RakX {
	struct OpenConnectionRequest1 {
		constexpr static u8 ID{ 0x5 };
		u8 protocol_version{ 11 };
		usize MTU{ 0 };
		constexpr static usize SIZE_OFFSET = UDP_HEADER_SIZE + sizeof(u8) + sizeof(Magic) + sizeof(u8);

	};
	namespace Detail {
		template<>
		inline OpenConnectionRequest1 parse_from_buffer<OpenConnectionRequest1>(RawBuffer& other, std::endian e) rel_noexcept {
			OpenConnectionRequest1 req{};
			(void)other.read<Magic>();
			req.protocol_version = other.read<u8>();
			usize x = 0;
			for (; other.read_byte() == 0; x++) {}
			req.MTU = x + req.SIZE_OFFSET;
			return req;
		}

		template<>
		inline void parse_to_buffer<OpenConnectionRequest1>(const OpenConnectionRequest1& self, RawBuffer& other, std::endian e) noexcept {
			other
				.write(self.ID)
				.write(Magic{})
				.write(self.protocol_version);
			for (u16 x = 0; x < self.MTU - self.SIZE_OFFSET; x++) {
				other.write<u8>(0);
			}
		}
	}
}