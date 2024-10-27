#pragma once
#include <RakX/common/ip.hpp>
#include <RakX/common/ptr.hpp>
#include <RakX/common/int.hpp>
#include <RakX/common/err.hpp>
#include <sockpp/udp_socket.h>

namespace RakX {
	class Server;
	class Connection {
	public:
		Connection(u64 guid, sockpp::inet_address addr, Server& send_server, u16 mtu) noexcept : m_client_guid(guid) {
			this->m_client_addr = Address{ IPv4{addr.address()}, addr.port() };
			this->m_send_server = &send_server;
			this->m_mtu = mtu;
		}

		inline IPv4 ip() const noexcept {
			return this->m_client_addr.ip;
		}

		inline u16 port() const noexcept {
			return this->m_client_addr.port;
		}

		inline u64 guid() const noexcept {
			return this->m_client_guid;
		}

		inline u16 mtu() const noexcept {
			return this->m_mtu;
		}

		inline sockpp::inet_address addr() const noexcept {
			return sockpp::inet_address(ip().combined(), port());
		}
		inline Address address() const noexcept {
			return Address{ sockpp::inet_address(ip().to_string(), port())};
		}

		inline void set_guid(u64 guid) noexcept {
			this->m_client_guid = guid;
		}


		std::expected<bool, Err<>>  process_rak_frame() noexcept;

	private:
		Address m_client_addr{};
		u64 m_client_guid{};
		u16 m_mtu{ 0 };
		AccessPtr<Server> m_send_server{};
	};
}