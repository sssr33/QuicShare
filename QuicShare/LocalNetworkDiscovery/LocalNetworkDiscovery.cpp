#include "LocalNetworkDiscovery.h"

LocalNetworkDiscovery::LocalNetworkDiscovery(boost::asio::io_context& ioContext_, const std::string& localId_)
    : ioContext(ioContext_)
    , localId(localId_)
{
    boost::asio::ip::udp::resolver resolver(ioContext);
    auto it = resolver.resolve(boost::asio::ip::host_name(), "", boost::asio::ip::resolver_base::flags::passive);

    for (const auto& e : it) {
        auto addr = e.endpoint().address();
        auto str = addr.to_string();
        auto isv4 = addr.is_v4();
        auto isLoopback = addr.is_loopback();

        if (!addr.is_loopback()) {
            auto lnd = std::make_unique<LocalNetworkDiscoveryChannel>(ioContext, addr, localId);
            connect(lnd.get(), &LocalNetworkDiscoveryChannel::NewPeerAvailable, this, &LocalNetworkDiscovery::NewPeerOnChannelAvailable);
            channels.push_back(std::move(lnd));
        }
    }
}

void LocalNetworkDiscovery::Announce() {
    for (auto& i : channels) {
        i->Announce();
    }
}

const std::map<std::string, LocalNetworkPeerInfo>& LocalNetworkDiscovery::GetPeers() const {
    return peers;
}

void LocalNetworkDiscovery::NewPeerOnChannelAvailable(const LocalNetworkChannelPeerInfo& info) {
    auto listen = info.listenAddress.to_string();
    auto endpoint = info.endpoint.address().to_string();
    bool isV4 = info.endpoint.address().is_v4();
    bool isV6 = info.endpoint.address().is_v6();

    auto it = peers.find(info.localId);
    if (it == peers.end()) {
        LocalNetworkPeerInfo newPeer;

        newPeer.self = info.localId == localId;
        newPeer.localId = info.localId;
        newPeer.listenAddress = info.listenAddress;
        newPeer.endpoints.push_back(info.endpoint);

        auto added = peers.emplace(info.localId, std::move(newPeer));

        emit LocalPeerAdded(added.first->second);
    }
    else {
        auto& existingPeer = it->second;
        auto& existingEndpoints = existingPeer.endpoints;

        auto it = std::find_if(existingEndpoints.begin(), existingEndpoints.end(),
            [&](const boost::asio::ip::udp::endpoint& endpoint)
            {
                return endpoint.address() == info.endpoint.address();
            }
        );

        if (it == existingEndpoints.end()) {
            // new endpoint
            existingEndpoints.push_back(info.endpoint);

            emit LocalPeerEndpointAdded(existingPeer, info.endpoint);
        }
        else {
            // existing endpoint
            return;
        }
    }
}
