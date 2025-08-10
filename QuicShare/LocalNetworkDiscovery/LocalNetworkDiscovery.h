#pragma once
#include "LocalNetworkDiscoveryChannel.h"
#include "LocalNetworkPeerInfo.h"

#include <QObject>
#include <vector>
#include <memory>
#include <map>

class LocalNetworkDiscovery : public QObject {
    Q_OBJECT
public:
    LocalNetworkDiscovery(boost::asio::io_context& ioContext_, const std::string& localId_);

    void Announce();

    const std::map<std::string, LocalNetworkPeerInfo>& GetPeers() const;

signals:
    void LocalPeerAdded(const LocalNetworkPeerInfo& peer);
    void LocalPeerPathAdded(const LocalNetworkPeerInfo& peer, LocalNetworkPeerPath path);

private slots:
    void NewPeerOnChannelAvailable(const LocalNetworkChannelPeerInfo& info);

private:
    boost::asio::io_context& ioContext;
    std::string localId;
    std::vector<std::unique_ptr<LocalNetworkDiscoveryChannel>> channels;
    std::map<std::string, LocalNetworkPeerInfo> peers;
};
