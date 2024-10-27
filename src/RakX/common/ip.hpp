#pragma once
#include <array>
#include <RakX/common/int.hpp>
#include <RakX/common/constants.hpp>
#include <RakX/common/parse_buffer.hpp>
#include <sockpp/udp_socket.h>
#include <expected>
#include <string_view>
#include <string>
#include <bit>
#include <xhash>

namespace RakX {

	namespace Detail {
		enum class IPParseError {
			SECTION_TOO_LARGE,
			MISSING_DOT,
			INVALID_NIBBLE_FORMAT
		};
	}

	class IPv4 {
	public:
		constexpr IPv4(u8 A, u8 B, u8 C, u8 D) {
			this->m_parts[0] = D;
			this->m_parts[1] = C;
			this->m_parts[2] = B;
			this->m_parts[3] = A;
		}
		constexpr IPv4(u32 whole) {
			this->m_combined = whole;
		}
		constexpr IPv4() = default;


		static std::expected<IPv4, Detail::IPParseError> from_string(std::string_view str) noexcept;

		std::string to_string() const noexcept;

		inline u32 combined() const noexcept { return this->m_combined; }
	private:

	private:
		union {
			std::array<u8, 4> m_parts;
			u32 m_combined = 0;
		};
	};

	struct Address {
		IPv4 ip{};
		u16 port{};
		Address() = default;
		Address(sockpp::inet_address addr) {
			this->ip = IPv4(addr.address());
			this->port = addr.port();
		}
		Address(IPv4 ip, u16 port) {
			this->ip = ip;
			this->port = port;
		}

		bool operator==(const Address& other) const {
			return other.ip.combined() == this->ip.combined() && other.port == this->port;
		}
	};

	namespace Detail {
		enum class IPVersion : u8 {
			IPv4 = 4,
			IPv6 = 6
		};

		template<>
		inline Address parse_from_buffer<Address>(RawBuffer& other, std::endian e) {
			IPVersion ver = (IPVersion)other.read_byte();
			if (ver != IPVersion::IPv4) {
				throw std::runtime_error("IPV6 not supported");
			}
			u32 whole = other.read<u32>();
			u16 port = other.read<u16>(std::endian::big);
			return Address{ IPv4{whole}, port };
		}
		template<>
		inline void parse_to_buffer<Address>(const Address& self, RawBuffer& other, std::endian e) noexcept {
			Address addr{};
			other.write(addr.ip.combined()).write(addr.port, std::endian::big);
		}
	}




	class IPv6 {
	public:
		constexpr IPv6(u64 global_routing_prefix, u16 subnet_id, u64 interface_id) noexcept;
		constexpr IPv6(std::array<u16, 3> global_routing_prefix, u16 subnet_id, std::array<u16, 4> interface_id) noexcept;
	private:
		std::array<u16, 3> m_global_routing_prefix{0x0};
		u16 m_subnet_identifier{ 0 };
		std::array<u16, 4> m_interface_id{ 0x0 };
	};
}

namespace std {
	using namespace RakX;
	template<>
	struct hash<RakX::Address> {
		usize operator()(const Address& addr) const {
			return std::hash<u32>{}(addr.ip.combined()) ^ std::hash<u16>{}(addr.port);
		}
	};
}