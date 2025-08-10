#include "LocalNetworkDiscoveryChannel.h"
#include "LndMessageAnnounce.h"

//constexpr auto addrV4 = "239.192.152.143"; // torrent protocol ipv4
//constexpr auto addrV6 = "ff15::efc0:988f"; // torrent protocol ipv6
constexpr auto addrV4 = "239.255.0.1"; // differs from torrent protocol
constexpr auto addrV6 = "ff15::cafe:babe"; // differs from torrent protocol
constexpr auto port = 30001; // differs from torrent protocol. torrent protocol uses 6771

LocalNetworkDiscoveryChannel::LocalNetworkDiscoveryChannel(
    boost::asio::io_context& ioContext,
    boost::asio::ip::address listenAddress_,
    const std::string& localId_
)
    : listenAddress(listenAddress_)
    , localId(localId_)
    , socket(ioContext)
    , socketReceiveBuffer(1024 * 4)
{
    using namespace boost::asio::ip;
    using namespace boost::asio::ip::multicast;

    const bool v4 = listenAddress.is_v4();

    udp::endpoint listen_endpoint(udp::v4(), port);

    socket.open(v4 ? udp::v4() : udp::v6());

    socket.set_option(udp::socket::reuse_address(true));

    socket.bind(udp::endpoint(v4 ? address(address_v4::any()) : address(address_v6::any()), port));

    if (v4) {
        socket.set_option(join_group(make_address(addrV4).to_v4(), listenAddress.to_v4()));
    }
    else {
        socket.set_option(join_group(make_address(addrV6).to_v6(), listenAddress.to_v6().scope_id()));
    }

    socket.set_option(hops(32));
    socket.set_option(enable_loopback(true));

    if (v4) {
        socket.set_option(outbound_interface(listenAddress.to_v4()));
    }

    socket.async_receive_from(
        boost::asio::buffer(socketReceiveBuffer),
        socketReceiveEndpoint,
        std::bind(&LocalNetworkDiscoveryChannel::ReceiveHandler, this, std::placeholders::_1, std::placeholders::_2)
    );
}

void LocalNetworkDiscoveryChannel::Announce() {
    using namespace boost::asio::ip;
    using namespace boost::asio::ip::multicast;

    const bool v4 = listenAddress.is_v4();
    auto multicastEndpoint = udp::endpoint(v4 ? address(make_address(addrV4).to_v4()) : address(make_address(addrV6).to_v6()), port);

    std::string message = "Hello, multicast! From: " + listenAddress.to_string();

    LndMessageAnnounce msg;

    msg.peerId = localId;

    auto jsonStr = JS::serializeStruct(msg, JS::SerializerOptions(JS::SerializerOptions::Compact));

    try {
        socket.send_to(boost::asio::buffer(jsonStr), multicastEndpoint);
        int stop = 234;
    }
    catch (const std::exception& ex) {
        auto what = ex.what();
        int stop = 34;
    }
}

void LocalNetworkDiscoveryChannel::ReceiveHandler(
    const boost::system::error_code& error,
    std::size_t bytesTransferred
)
{
    const bool v4 = listenAddress.is_v4();
    auto addrStr = listenAddress.to_string();

    if (error) {
        int stop = 234;
        assert(false);
        return;
    }

    std::string msgStr = std::string{ socketReceiveBuffer.begin(), socketReceiveBuffer.begin() + bytesTransferred };

    LndMessage msgBase;

    {
        JS::ParseContext context(msgStr);
        auto parseError = context.parseTo(msgBase);
        if (parseError != JS::Error::NoError) {
            assert(false);
            return;
        }
    }

    JS::ParseContext context(msgStr);

    switch (msgBase.type) {
    case LndMessageType::Announce: {
        LndMessageAnnounce msg;
        auto parseError = context.parseTo(msg);
        if (parseError != JS::Error::NoError) {
            assert(false);
            return;
        }

        LocalNetworkPeerInfo peerInfo;

        peerInfo.localId = msg.peerId;
        peerInfo.listenAddress = listenAddress;
        peerInfo.endpoint = socketReceiveEndpoint;

        emit NewPeerAvailable(peerInfo);

        break;
    }
    default:
        assert(false);
        break;
    }

    socket.async_receive_from(
        boost::asio::buffer(socketReceiveBuffer),
        socketReceiveEndpoint,
        std::bind(&LocalNetworkDiscoveryChannel::ReceiveHandler, this, std::placeholders::_1, std::placeholders::_2)
    );
}
