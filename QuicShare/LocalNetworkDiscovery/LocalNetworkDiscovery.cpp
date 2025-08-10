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

void LocalNetworkDiscovery::NewPeerOnChannelAvailable(const LocalNetworkChannelPeerInfo& info) {
    auto listen = info.listenAddress.to_string();
    auto endpoint = info.endpoint.address().to_string();

    if (info.localId == localId) {
        int stop = 234;
    }
    else {
        int stop = 234;
    }
}
