#pragma once
#include "LocalNetworkPeerPath.h"

#include <vector>

struct LocalNetworkPeerInfo {
    // True if this LocalNetworkPeerInfo instance describes the local client itself
    // (possible when loopback discovery is enabled, so the client detects its own info)
    bool self = false;
    std::string localId;
    std::vector<LocalNetworkPeerPath> paths;
};
