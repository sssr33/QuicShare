#pragma once
#include "SafeBoostAsioInclude.h"

#include <string>

struct LocalNetworkPeerInfo {
    std::string localId;
    boost::asio::ip::udp::endpoint endpoint;
    boost::asio::ip::address listenAddress;
};
