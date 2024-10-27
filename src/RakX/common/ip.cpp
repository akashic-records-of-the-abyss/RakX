#include "ip.hpp"
#include <string>
#include <format>
#include <RakX/common/constants.hpp>
#include <charconv> 

namespace RakX
{
    std::expected < IPv4, Detail::IPParseError > IPv4::from_string(std::string_view str) noexcept
    {
        IPv4 ret;
        std::string_view work_str(str);
        for (u8 x = 0; x < 4; x++) {
            usize dot_pos = 0;
            if (x != 3) {
                dot_pos = work_str.find('.');
            }
            else {
                dot_pos = work_str.size();
            }
            if (dot_pos > 3)
                return std::unexpected(Detail::IPParseError::SECTION_TOO_LARGE);
            if (dot_pos == std::string::npos)
                return std::unexpected(Detail::IPParseError::MISSING_DOT);

            const auto number_str = work_str.substr(0, dot_pos);
            if (x != 3) {
                work_str = work_str.substr(dot_pos + 1);
            }
            u8 nibble = 0;
            const auto [ptr, ec] = std::from_chars(number_str.data(), number_str.data() + number_str.size(), nibble);
            if (ec == std::errc()) {
                ret.m_parts[3 - x] = nibble;
            }
            else {
                return std::unexpected(Detail::IPParseError::INVALID_NIBBLE_FORMAT);
            }
        }
        return ret;
    }
    std::string IPv4::to_string() const noexcept
    {
        return std::format("{}.{}.{}.{}", this->m_parts[3], this->m_parts[2], this->m_parts[1], this->m_parts[0]);
    }
    constexpr IPv6::IPv6(u64 global_routing_prefix, u16 subnet_id, u64 interface_id) noexcept
    {
        global_routing_prefix = as_endian<std::endian::big>(global_routing_prefix);
        global_routing_prefix >>= 16;
        global_routing_prefix <<= 16;
        const auto grp_hexts = std::bit_cast<std::array<u16, 4>>(global_routing_prefix);
        interface_id = as_endian<std::endian::big>(interface_id);
        const auto interface_id_hexts = std::bit_cast<std::array<u16, 4>>(interface_id);
        this->m_global_routing_prefix = { grp_hexts[0], grp_hexts[1], grp_hexts[2] };
        this->m_subnet_identifier = subnet_id;
        this->m_interface_id = interface_id_hexts;
        
    }
    constexpr IPv6::IPv6(std::array<u16, 3> global_routing_prefix, u16 subnet_id, std::array<u16, 4> interface_id) noexcept
    {
        this->m_global_routing_prefix = global_routing_prefix;
        this->m_subnet_identifier = subnet_id;
        this->m_interface_id = interface_id;
    }
}
