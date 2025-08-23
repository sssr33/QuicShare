#pragma once
#include "SafeBoostAsioInclude.h"

#include <string>

struct LocalNetworkChannelPeerInfo {
    std::string localId;
    uint16_t quicPort = 0;
    boost::asio::ip::address listenAddress;
    boost::asio::ip::udp::endpoint endpoint;
};
