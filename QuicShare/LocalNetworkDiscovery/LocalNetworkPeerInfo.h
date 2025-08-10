#pragma once
#include "SafeBoostAsioInclude.h"

#include <string>
#include <vector>

struct LocalNetworkPeerInfo {
    // True if this LocalNetworkPeerInfo instance describes the local client itself
    // (possible when loopback discovery is enabled, so the client detects its own info)
    bool self = false;
    std::string localId;
    // Address of the local network adapter that received the request for local network discovery.
    // When communicating with this peer, it may be preferable to send requests from this address.
    boost::asio::ip::address listenAddress;
    std::vector<boost::asio::ip::udp::endpoint> endpoints;
};
