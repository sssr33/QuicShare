#pragma once
#include "SafeBoostAsioInclude.h"

#include <string>

struct LocalNetworkPeerPath {
    bool operator==(const LocalNetworkPeerPath&) const = default;

    std::string ToString() const;

    // Address of the local network adapter that received the request for local network discovery.
    // When communicating with this peer, it may be preferable to send requests from this address.
    boost::asio::ip::address listenAddress;
    boost::asio::ip::udp::endpoint endpoint;
    uint16_t quicPort = 0;
};
