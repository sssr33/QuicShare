#include "LocalNetworkPeerPath.h"

#include <format>

std::string LocalNetworkPeerPath::ToString() const {
    auto str = std::format("l: {} - ep: {}:{}", listenAddress.to_string(), endpoint.address().to_string(), endpoint.port());
    return str;
}
