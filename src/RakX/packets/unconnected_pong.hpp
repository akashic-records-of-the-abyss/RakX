#pragma once
#include <RakX/common/int.hpp>
#include <RakX/common/constants.hpp>
#include <RakX/common/parse_buffer.hpp>
#include <RakX/packets/unconnected_ping.hpp>
#include <string>

namespace RakX {

	struct UnconnectedPong {
		static constexpr u8 ID = 0x1c;
		u64 client_alive_time{ 0 };
		u64 server_guid{ 0 };
		std::string server_id{};
		UnconnectedPong() = default;
		UnconnectedPong(const UnconnectedPing& ping, const std::string& message, u64 serverGUID = 0xDEADC0DE) {
			this->client_alive_time = ping.client_alive_time;
			this->server_guid = serverGUID;
			this->server_id = message;
		}
	};

	namespace Detail {
		template<>
		inline UnconnectedPong parse_from_buffer<UnconnectedPong>(RawBuffer& other, std::endian e) rel_noexcept {
			UnconnectedPong pong{};
			pong.client_alive_time = other.read<u64>();
			pong.server_guid = other.read<u64>();
			(void)other.read<Magic>();
			pong.server_id = other.read<std::string>();
			return pong;
		}

		template<>
		inline void parse_to_buffer<UnconnectedPong>(const UnconnectedPong& self, RawBuffer& other, std::endian e) noexcept {
			other
				.write<u8>(self.ID)
				.write(self.client_alive_time, std::endian::big)
				.write(self.server_guid, std::endian::big)
				.write(Magic{})
				.write(self.server_id);
		}
	}
}