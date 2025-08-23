#include "LocalNetworkDiscovery.h"

LocalNetworkDiscovery::LocalNetworkDiscovery(
    boost::asio::io_context& ioContext_,
    const std::string& localId_,
    const std::vector<LocalNetworkDiscoveryEndpoint>& listenEndpoints
)
    : ioContext(ioContext_)
    , localId(localId_)
{
    for (auto& e : listenEndpoints) {
        auto addr = e.endpoint.address();

        auto lnd = std::make_unique<LocalNetworkDiscoveryChannel>(ioContext, addr, localId, e.quicPort);
        connect(lnd.get(), &LocalNetworkDiscoveryChannel::NewPeerAvailable, this, &LocalNetworkDiscovery::NewPeerOnChannelAvailable);
        channels.push_back(std::move(lnd));
    }
}

const std::map<std::string, LocalNetworkPeerInfo>& LocalNetworkDiscovery::GetPeers() const {
    return peers;
}

std::vector<boost::asio::ip::address> LocalNetworkDiscovery::GetListenAddresses() const {
    std::vector<boost::asio::ip::address> addresses;

    for (auto& i : channels) {
        addresses.push_back(i->GetListenAddress());
    }

    return addresses;
}

void LocalNetworkDiscovery::Announce() {
    for (auto& i : channels) {
        i->Announce();
    }
}

void LocalNetworkDiscovery::NewPeerOnChannelAvailable(const LocalNetworkChannelPeerInfo& info) {
    auto listen = info.listenAddress.to_string();
    auto endpoint = info.endpoint.address().to_string();
    bool isV4 = info.endpoint.address().is_v4();
    bool isV6 = info.endpoint.address().is_v6();

    auto it = peers.find(info.localId);
    if (it == peers.end()) {
        LocalNetworkPeerInfo newPeer;

        auto newPath = LocalNetworkPeerPath{
            .listenAddress = info.listenAddress,
            .endpoint = info.endpoint,
            .quicPort = info.quicPort
        };

        newPeer.self = info.localId == localId;
        newPeer.localId = info.localId;
        newPeer.paths.push_back(newPath);

        auto added = peers.emplace(info.localId, std::move(newPeer));

        emit LocalPeerAdded(added.first->second);
    }
    else {
        auto& existingPeer = it->second;
        auto& existingPaths = existingPeer.paths;
        auto newPath = LocalNetworkPeerPath{
            .listenAddress = info.listenAddress,
            .endpoint = info.endpoint,
            .quicPort = info.quicPort
        };

        auto it = std::find(existingPaths.begin(), existingPaths.end(), newPath);

        if (it == existingPaths.end()) {
            auto str = newPath.ToString();

            // new path
            existingPaths.push_back(std::move(newPath));

            emit LocalPeerPathAdded(existingPeer, existingPaths.back());
        }
        else {
            // existing path
            return;
        }
    }
}
