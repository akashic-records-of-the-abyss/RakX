#include "connection.hpp"
#include <RakX/server.hpp>

namespace RakX {
	std::expected<bool, Err<>> Connection::process_rak_frame() noexcept
	{
		auto& buff = this->m_send_server->fetch_buffer();
		const u8 id = buff.read_byte();
		if ((id & this->m_send_server->VALID_PACKET) != this->m_send_server->VALID_PACKET)
			return;



		return false;
	}
}