#pragma once
#include <RakX/common/int.hpp>
#include <RakX/common/constants.hpp>
#include <RakX/common/parse_buffer.hpp>


namespace RakX {
	struct IncompatibleProtocol {
		constexpr static u8 ID = 0x19;
		u8 expected_version{11};
		u64 server_guid{ 0 };
	};

	namespace Detail {
		template<>
		inline IncompatibleProtocol parse_from_buffer<IncompatibleProtocol>(RawBuffer& other, std::endian e) rel_noexcept {
			IncompatibleProtocol pro{};
			pro.expected_version = other.read_byte();
			pro.server_guid = other.read<u64>(std::endian::big);
			return pro;
		}

		template<>
		inline void parse_to_buffer<IncompatibleProtocol>(const IncompatibleProtocol& self, RawBuffer& other, std::endian e) noexcept {
			other.write(self.ID).write(self.expected_version).write(Magic{}).write(self.server_guid, std::endian::big);
		}
	}
}