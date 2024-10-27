#pragma once
#include <RakX/common/int.hpp>
#include <RakX/common/constants.hpp>
#include <RakX/common/parse_buffer.hpp>

namespace RakX {
	struct OpenConnectionReply1 {
		constexpr static u8 ID = 0x6;
		u64 server_guid{ 0 };
		u16 MTU{ 0 };
	};

	namespace Detail {
		template<>
		inline OpenConnectionReply1 parse_from_buffer<OpenConnectionReply1>(RawBuffer& other, std::endian e) rel_noexcept {
			OpenConnectionReply1 req{};
			(void)other.read<Magic>();
			req.server_guid = other.read<u64>(std::endian::big);
			const bool cookie = other.read<bool>();
			if (cookie) {
				throw std::runtime_error("Cookie Not Supported");
			}
			req.MTU = other.read<u16>(std::endian::big);
			return req;
		}

		template<>
		inline void parse_to_buffer<OpenConnectionReply1>(const OpenConnectionReply1& self, RawBuffer& other, std::endian e) noexcept {
			other
				.write(self.ID)
				.write(Magic{})
				.write(self.server_guid, std::endian::big)
				.write<bool>(false)
				.write(self.MTU, std::endian::big);
		}
	}
}