#pragma once
#include "ui_QuicShare.h"
#include "LocalNetworkDiscovery/LocalNetworkDiscovery.h"
#include "QUIC/MsQuicListener.h"

#include <QtWidgets/QMainWindow>
#include <vector>
#include <memory>
#include <thread>

class QuicShare : public QMainWindow {
    Q_OBJECT
public:
    QuicShare(QWidget *parent = nullptr);
    ~QuicShare();

public slots:
    void LocalPeerAdded(const LocalNetworkPeerInfo& peer);
    void LocalPeerPathAdded(const LocalNetworkPeerInfo& peer, LocalNetworkPeerPath path);

private:
    void OnStartClicked();
    void IoContextThreadMain();

    void StartQuicListeners(const std::vector<boost::asio::ip::udp::endpoint>& adapterEndpoints);
    void StartLocalNetworkDiscovery();

    Ui::QuicShareClass ui;

    boost::asio::io_context ioContext;
    std::unique_ptr<LocalNetworkDiscovery> localNetworkDiscovery;
    std::string localId;

    std::vector<std::unique_ptr<MsQuicListener>> quicListeners;

    std::jthread ioContextThread;
};
