#pragma once
#include "SafeBoostAsioInclude.h"

struct LocalNetworkDiscoveryEndpoint {
    boost::asio::ip::udp::endpoint endpoint;
    uint16_t quicPort = 0;
};
