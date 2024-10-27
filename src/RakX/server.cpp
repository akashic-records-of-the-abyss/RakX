#include "server.hpp"
#include <print>
#include <RakX/packets/unconnected_ping.hpp>
#include <RakX/packets/unconnected_pong.hpp>
#include <RakX/packets/open_connection_request1.hpp>
#include <RakX/packets/open_connection_reply1.hpp>
#include <RakX/packets/open_connection_request2.hpp>
#include <RakX/packets/open_connection_reply2.hpp>
#include <RakX/packets/incompatible_protocol.hpp>

namespace RakX
{
	Err<> Server::init_server(ServerInitConfig cfg) noexcept
	{
		if (this->m_has_been_init) {
			return Err<>("Server Already Init");
		}
		sockpp::udp_socket server{};
		if (!server) {
			return Err<>(std::format("Failed to instance server! Error: {}", server.last_error_str()));
		}

		sockpp::inet_address addr(cfg.bind_target.to_string(), cfg.port);
		if (!server.bind(addr)) {
			return Err<>(std::format("Failed to bind server! Error: {}", server.last_error_str()));
		}
		this->m_server = std::move(server);
		this->m_has_been_init = true;
		return Err();
	}

	void Server::internal_start() noexcept
	{
		std::println("Listening for clients!");
		this->m_uptime = std::chrono::steady_clock::now();
		while (this->m_run) {

			auto curr_time = std::chrono::steady_clock::now();
			auto difference = curr_time - this->m_uptime;
			this->m_uptime += difference;

			this->m_packet_buffer.wipe();
			sockpp::inet_address client_addr{};
			isize byte_count = this->m_server.recv_from(static_cast<void*>(this->m_packet_buffer.raw_data()), sizeof(this->m_packet_buffer), &client_addr);
			if (byte_count < 0) {
				std::println("Failed to recv data from client: {}", this->m_server.last_error_str());
			}

			this->m_packet_buffer.top(byte_count);

			if (process_internal_packets(client_addr))
				continue;
			else if (const auto& val = this->m_connections.find(Address{ client_addr }); val != this->m_connections.end()) {
				if (val->second.process_rak_frame())
					continue;
			}

			this->m_packet_buffer.dump(1);
		}
	}


	bool Server::process_internal_packets(const sockpp::inet_address& addr) noexcept
	{
		const u8 rak_id = this->m_packet_buffer.read<u8>();
		switch (rak_id) {
		case UnconnectedPing::ID:
		case UnconnectedPing::ID2: {
			const auto ping = this->m_packet_buffer.read<UnconnectedPing>();
			this->m_packet_buffer.wipe();
			this->m_packet_buffer.write(UnconnectedPong{ ping, m_server_message });
			this->send_raw(addr);
			return true;
		}
		case OpenConnectionRequest1::ID: {
			process_connection1(addr);
			return true;
		}
		case OpenConnectionRequest2::ID: {
			process_connection2(addr);
			return true;
		}
		}
		this->m_packet_buffer.wipe();
		return false;

	}
	void Server::process_connection1(const sockpp::inet_address& addr) noexcept
	{
		const auto con = this->m_packet_buffer.read<OpenConnectionRequest1>();

		if (con.protocol_version != this->m_rak_proto) {
			std::println("Raknet version mismatch!");
			this->m_packet_buffer.wipe();
			this->m_packet_buffer.write(IncompatibleProtocol{ this->m_rak_proto, this->m_server_guid });
			this->send_raw(addr);
			return;
		}
		auto connection = Connection(-1, addr, *this, con.MTU);
		this->m_candidate_connections.insert({ Address{ addr }, connection });

		this->m_packet_buffer.wipe();
		this->m_packet_buffer.write(OpenConnectionReply1{ this->m_server_guid, (u16)con.MTU });
		this->send_raw(addr);
	}
	void Server::process_connection2(const sockpp::inet_address& addr) noexcept
	{
		if (this->m_candidate_connections.find(Address{ addr }) == this->m_candidate_connections.end()) {
			return;
		}
		const auto req = this->m_packet_buffer.read<OpenConnectionRequest2>();
		auto con = std::move(this->m_candidate_connections.find(Address{ addr })->second);
		this->m_candidate_connections.erase(Address{ addr });
		con.set_guid(req.clien_guid);
		this->m_packet_buffer.wipe();
		this->m_packet_buffer.write(
			OpenConnectionReply2{ .server_guid = this->m_server_guid, .client_address = con.address(), .MTU = req.MTU, .encryption = false }
		);
		this->m_connections.insert({ Address{addr}, std::move(con) });
		this->m_packet_buffer.dump(this->m_packet_buffer.size());
		this->send_raw(addr);
	}
	void Server::send_raw(const sockpp::inet_address& addr)
	{
		this->m_server.send_to(this->m_packet_buffer.raw_data(), this->m_packet_buffer.size(), addr);
	}
}
