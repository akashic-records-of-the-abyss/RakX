#pragma once
#include <RakX/common/int.hpp>
#include <RakX/common/ip.hpp>
#include <RakX/common/constants.hpp>
#include <RakX/common/parse_buffer.hpp>

namespace RakX {
	struct OpenConnectionRequest2 {
		constexpr static u8 ID{ 0x7 };
		u16 MTU{ 0 };
		u64 clien_guid{ 0 };
		Address server_addr{};
	};
	namespace Detail {
		template<>
		inline OpenConnectionRequest2 parse_from_buffer<OpenConnectionRequest2>(RawBuffer& other, std::endian e) rel_noexcept {
			OpenConnectionRequest2 req{};
			(void)other.read<Magic>();
			req.server_addr = other.read<Address>();
			req.MTU = other.read<u16>(std::endian::big);
			req.clien_guid = other.read<u64>(std::endian::big);
			return req;
		}

		template<>
		inline void parse_to_buffer<OpenConnectionRequest2>(const OpenConnectionRequest2& self, RawBuffer& other, std::endian e) noexcept {
			other.write(self.ID).write(Magic{}).write(self.server_addr).write(self.MTU, std::endian::big).write(self.clien_guid, std::endian::big);
		}
	}
}