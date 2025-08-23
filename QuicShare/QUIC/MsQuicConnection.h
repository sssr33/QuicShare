#pragma once
#include "MsQuic.h"
#include "LocalNetworkDiscovery/LocalNetworkPeerPath.h"

class MsQuicConnection {
public:
    MsQuicConnection();

    void Connect(const LocalNetworkPeerPath& path);
    void ConnectFromListener(HQUIC connection_, HQUIC config);

private:
    static QUIC_STATUS QUIC_API QuicConnectionCallback(
        _In_ HQUIC Connection,
        _In_opt_ void* Context,
        _Inout_ QUIC_CONNECTION_EVENT* Event
    );

    HQUIC connection = {};
};
