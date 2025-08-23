#pragma once
#include "SafeBoostAsioInclude.h"
#include "LocalNetworkChannelPeerInfo.h"

#include <QObject>
#include <vector>
#include <thread>

class LocalNetworkDiscoveryChannel : public QObject {
    Q_OBJECT
public:
    LocalNetworkDiscoveryChannel(
        boost::asio::io_context& ioContext,
        boost::asio::ip::address listenAddress_,
        const std::string& localId_,
        uint16_t quicPort_
    );

    const boost::asio::ip::address& GetListenAddress() const;

    void Announce();

signals:
    void NewPeerAvailable(LocalNetworkChannelPeerInfo info);

private:
    void ReceiveHandler(
        const boost::system::error_code& error,
        std::size_t bytesTransferred
    );

    const boost::asio::ip::address listenAddress;
    const std::string localId;
    const uint16_t quicPort;

    boost::asio::ip::udp::socket socket;
    std::vector<uint8_t> socketReceiveBuffer;
    boost::asio::ip::udp::endpoint socketReceiveEndpoint;
};
