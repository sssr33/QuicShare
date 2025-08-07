#pragma once

// order in which boost sees the correct value of _WIN32_WINNT
// https://stackoverflow.com/questions/9750344/boostasio-winsock-and-winsock-2-compatibility-issue
// https://github.com/boostorg/beast/issues/1895
#ifdef _WIN32
#include <winsock2.h>
#endif
#include <boost/asio.hpp>

#include <vector>
#include <thread>

class LocalNetworkDiscovery {
public:
    LocalNetworkDiscovery(boost::asio::io_context& ioContext, boost::asio::ip::address listenAddress_);

    void Test();

private:
    void ReceiveHandler(
        const boost::system::error_code& error,
        std::size_t bytesTransferred
    );

    const boost::asio::ip::address listenAddress;

    boost::asio::ip::udp::socket socket;
    std::vector<uint8_t> socketReceiveBuffer;
    boost::asio::ip::udp::endpoint socketReceiveEndpoint;
};
