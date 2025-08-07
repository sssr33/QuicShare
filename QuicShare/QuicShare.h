#pragma once
#include "ui_QuicShare.h"
#include "LocalNetworkDiscovery/LocalNetworkDiscoveryChannel.h"

#include <QtWidgets/QMainWindow>
#include <vector>
#include <memory>

class QuicShare : public QMainWindow {
    Q_OBJECT
public:
    QuicShare(QWidget *parent = nullptr);
    ~QuicShare();

private:
    void OnStartClicked();
    void IoContextThreadMain();

    Ui::QuicShareClass ui;

    boost::asio::io_context ioContext;
    std::thread ioContextThread;
    std::vector<std::unique_ptr<LocalNetworkDiscoveryChannel>> localNetworkDiscovery;
};
