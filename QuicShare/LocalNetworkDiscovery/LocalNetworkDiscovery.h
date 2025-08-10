#pragma once
#include "LocalNetworkDiscoveryChannel.h"

#include <QObject>
#include <vector>
#include <memory>

class LocalNetworkDiscovery : public QObject {
    Q_OBJECT
public:
    LocalNetworkDiscovery(boost::asio::io_context& ioContext_, const std::string& localId_);

    void Announce();

private slots:
    void NewPeerOnChannelAvailable(const LocalNetworkChannelPeerInfo& info);

private:
    boost::asio::io_context& ioContext;
    std::string localId;
    std::vector<std::unique_ptr<LocalNetworkDiscoveryChannel>> channels;
};
