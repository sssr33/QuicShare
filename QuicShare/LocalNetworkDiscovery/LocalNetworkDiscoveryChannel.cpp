#include "LocalNetworkDiscoveryChannel.h"

//constexpr auto addrV4 = "239.192.152.143"; // torrent protocol ipv4
//constexpr auto addrV6 = "ff15::efc0:988f"; // torrent protocol ipv6
constexpr auto addrV4 = "239.255.0.1"; // differs from torrent protocol
constexpr auto addrV6 = "ff15::cafe:babe"; // differs from torrent protocol
constexpr auto port = 30001; // differs from torrent protocol. torrent protocol uses 6771

LocalNetworkDiscoveryChannel::LocalNetworkDiscoveryChannel(boost::asio::io_context& ioContext, boost::asio::ip::address listenAddress_)
    : listenAddress(listenAddress_)
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

void LocalNetworkDiscoveryChannel::Test() {
    using namespace boost::asio::ip;
    using namespace boost::asio::ip::multicast;

    const bool v4 = listenAddress.is_v4();
    auto multicastEndpoint = udp::endpoint(v4 ? address(make_address(addrV4).to_v4()) : address(make_address(addrV6).to_v6()), port);

    std::string message = "Hello, multicast! From: " + listenAddress.to_string();

    try {
        socket.send_to(boost::asio::buffer(message), multicastEndpoint);
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
    }

    std::string msg = std::string{ socketReceiveBuffer.begin(), socketReceiveBuffer.begin() + bytesTransferred };

    int stop = 234;

    socket.async_receive_from(
        boost::asio::buffer(socketReceiveBuffer),
        socketReceiveEndpoint,
        std::bind(&LocalNetworkDiscoveryChannel::ReceiveHandler, this, std::placeholders::_1, std::placeholders::_2)
    );
}
