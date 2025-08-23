#include "MsQuicHelpers.h"

QUIC_ADDR MsQuicHelpers::ToQuicAddr(const boost::asio::ip::address& addr) {
    QUIC_ADDR quicAddr{};

    if (addr.is_v4()) {
        const auto b = addr.to_v4().to_bytes();

        if (sizeof(quicAddr.Ipv4.sin_addr) != b.size()) {
            assert(false);
            return {};
        }

        assert(sizeof(quicAddr.Ipv4.sin_addr) == b.size());

        quicAddr.si_family = QUIC_ADDRESS_FAMILY_INET;
        std::memcpy(&quicAddr.Ipv4.sin_addr, b.data(), b.size());
    }
    else {
        const auto v6 = addr.to_v6();
        const auto b = v6.to_bytes();

        if (sizeof(quicAddr.Ipv6.sin6_addr) != b.size()) {
            assert(false);
            return {};
        }

        quicAddr.si_family = QUIC_ADDRESS_FAMILY_INET6;
        std::memcpy(&quicAddr.Ipv6.sin6_addr, b.data(), b.size());

        uint32_t sid = v6.scope_id();
        if (sid != 0) {
            quicAddr.Ipv6.sin6_scope_id = sid;
        }
    }

    return quicAddr;
}
