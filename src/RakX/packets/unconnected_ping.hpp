#pragma once
#include <RakX/common/int.hpp>
#include <RakX/common/constants.hpp>
#include <RakX/common/parse_buffer.hpp>

namespace RakX {

	struct UnconnectedPing {
		static constexpr u8 ID = 0x1;
		static constexpr u8 ID2 = 0x2;
		u8 self_id = ID;
		u64 client_alive_time{ 0 };
		u64 client_guid{ 0 };
	};

	namespace Detail {
		template<>
		inline UnconnectedPing parse_from_buffer<UnconnectedPing>(RawBuffer& other, std::endian e) rel_noexcept {
			UnconnectedPing ping{};
			ping.client_alive_time = other.read<u64>(std::endian::big);
			(void)other.read<Magic>();
			ping.client_guid = other.read<u64>(std::endian::big);
			return ping;
		}

		template<>
		inline void parse_to_buffer<UnconnectedPing>(const UnconnectedPing& self, RawBuffer& other, std::endian e) noexcept {
			other
				.write<u8>(self.self_id) // This is a variable so people can change it 
				.write(self.client_alive_time, std::endian::big)
				.write(Magic{})
				.write(self.client_guid);
		}
	}
}