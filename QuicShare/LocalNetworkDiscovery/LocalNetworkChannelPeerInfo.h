#pragma once
#include "SafeBoostAsioInclude.h"

#include <string>

struct LocalNetworkChannelPeerInfo {
    std::string localId;
    boost::asio::ip::address listenAddress;
    boost::asio::ip::udp::endpoint endpoint;
};
