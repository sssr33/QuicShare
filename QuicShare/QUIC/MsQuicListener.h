#pragma once
#include "MsQuic.h"
#include "MsQuicConnection.h"

#include <algorithm>
#include <string_view>

class MsQuicListener {
public:
    MsQuicListener(
        const boost::asio::ip::udp::endpoint& listenEndpoint_
    );

    const boost::asio::ip::udp::endpoint& GetListenEndpoint() const;
    uint16_t GetListenPort() const;

private:
    void StartListen(const boost::asio::ip::address& listenAddress);

    QUIC_STATUS ListenerCallback(
        HQUIC listener_,
        QUIC_LISTENER_EVENT* event
    );

    static QUIC_STATUS QUIC_API ListenerCallbackStatic(
        HQUIC listener,
        void* context,
        QUIC_LISTENER_EVENT* event
        );

    HQUIC configuration = nullptr;
    HQUIC listener = nullptr;
    boost::asio::ip::udp::endpoint listenEndpoint;
    uint16_t listenPort = 0;

    std::unique_ptr<MsQuicConnection> testConnection;
};
