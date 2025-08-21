#pragma once
#include "MsQuic.h"

#include <algorithm>
#include <string_view>

class MsQuicListener {
public:
    MsQuicListener(
        const boost::asio::ip::address& listenAddress,
        const char* certPath,
        const char* keyPath
    );

private:
    void InitConfiguration();
    void InitCredentials(
        const char* certPath,
        const char* keyPath
    );

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

    static QUIC_ADDR ToQuicAddr(const boost::asio::ip::address& addr);

    static constexpr const std::string_view AlpnStr = "qs/1";
    static const QUIC_BUFFER Alpn;

    HQUIC configuration = nullptr;
    HQUIC listener = nullptr;
};
