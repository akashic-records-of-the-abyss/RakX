#pragma once
#include <RakX/common/ip.hpp>
#include <RakX/common/int.hpp>
#include <RakX/common/parse_buffer.hpp>
#include <sockpp/udp_socket.h>
#include <RakX/common/err.hpp>
#include <RakX/connection.hpp>
#include <thread>
#include <unordered_map>
namespace RakX 
{

	struct ServerInitConfig {
		IPv4 bind_target = IPv4();
		u16 port = 0;

		ServerInitConfig(IPv4 bind_target = IPv4(), u16 port = 0) : bind_target(bind_target), port(port) {}
	};
	class Server 
	{
	public:
		Server() { sockpp::socket_initializer::initialize(); };

		Err<> init_server(ServerInitConfig cfg) noexcept;
		inline Err<> start() noexcept {
			if (!this->m_has_been_init) {
				return Err<>("Please init server before starting");
			}

			this->m_listener_thread = std::thread(&Server::internal_start, this);
			return Err<>();
		}
		inline void shutdown() noexcept {
			this->m_run = false;
			this->m_listener_thread.join();
		}
		inline void set_proto(u8 ver) noexcept {
			this->m_rak_proto = ver;
		}

		inline RawBuffer& fetch_buffer() noexcept {
			return this->m_packet_buffer;
		}
	public:
		static constexpr u8 VALID_PACKET{ 0x80 };

	private:
		void internal_start() noexcept;
		bool process_internal_packets(const sockpp::inet_address& addr) noexcept;
		void process_connection1(const sockpp::inet_address& addr) noexcept;
		void process_connection2(const sockpp::inet_address& addr) noexcept;
		void send_raw(const sockpp::inet_address& addr);
	private:
		ServerInitConfig m_config{};
		bool m_has_been_init = false;
		bool m_run = true;
		u8 m_rak_proto{ 11 };
		RawBuffer m_packet_buffer;
		sockpp::udp_socket m_server;
		std::thread m_listener_thread;
		std::string m_server_message{ "MCPE;Dedicated Server;758;1.21.31;0;10;2131;Bedrock level;Survival;1;19132;19133" };
		u64 m_server_guid{ 1237981 };
		std::chrono::time_point<std::chrono::steady_clock> m_uptime{};
		std::unordered_map<Address, Connection> m_candidate_connections{};
		std::unordered_map<Address, Connection> m_connections{};
	};

}

