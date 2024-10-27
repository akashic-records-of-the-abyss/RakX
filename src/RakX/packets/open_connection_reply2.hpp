#pragma once
#include <RakX/common/int.hpp>
#include <RakX/common/ip.hpp>
#include <RakX/common/constants.hpp>
#include <RakX/common/parse_buffer.hpp>

namespace RakX {
	struct OpenConnectionReply2 {
		constexpr static u8 ID = 0x8;
		u64 server_guid{ 0 };
		Address client_address{};
		u16 MTU{ 0 };
		bool encryption{false};
	};

	namespace Detail {
		template<>
		inline OpenConnectionReply2 parse_from_buffer<OpenConnectionReply2>(RawBuffer& other, std::endian e) rel_noexcept {
			OpenConnectionReply2 rep{};
			rep.server_guid = other.read<u64>(std::endian::big);
			rep.client_address = other.read<Address>();
			rep.MTU = other.read<u16>(std::endian::big);
			rep.encryption = other.read_byte() > 0;
			return rep;
		}

		template<>
		inline void parse_to_buffer<OpenConnectionReply2>(const OpenConnectionReply2& self, RawBuffer& other, std::endian e) noexcept {
			other
				.write(self.ID)
				.write(Magic{})
				.write(self.server_guid, std::endian::big)
				.write(self.client_address)
				.write(self.MTU)
				.write(self.encryption);
		}
	}
}