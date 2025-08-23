#include "MsQuicConnection.h"
#include "MsQuicHelpers.h"

MsQuicConnection::MsQuicConnection() {

}

void MsQuicConnection::Connect(const LocalNetworkPeerPath& path) {
    QUIC_STATUS quicRes = QUIC_STATUS_SUCCESS;
    auto& msQuicInstance = MsQuic::GetInstance();
    auto msQuic = msQuicInstance.GetMsQuic();
    auto registration = msQuicInstance.GetRegistration();

    quicRes = msQuic->ConnectionOpen(registration, &MsQuicConnection::QuicConnectionCallback, this, &connection);
    if (QUIC_FAILED(quicRes)) {
        assert(false);
        return;
    }

    auto localAddr = MsQuicHelpers::ToQuicAddr(path.listenAddress);

    quicRes = msQuic->SetParam(connection, QUIC_PARAM_CONN_LOCAL_ADDRESS, static_cast<uint32_t>(sizeof(localAddr)), &localAddr);
    if (QUIC_FAILED(quicRes)) {
        assert(false);
        return;
    }

    auto remoteAddr = path.endpoint.address();

    QUIC_ADDRESS_FAMILY addrFamily = QUIC_ADDRESS_FAMILY_UNSPEC;

    if (remoteAddr.is_v4()) {
        addrFamily = QUIC_ADDRESS_FAMILY_INET;
    }
    else if (remoteAddr.is_v6()) {
        addrFamily = QUIC_ADDRESS_FAMILY_INET6;
    }
    else {
        assert(false);
        return;
    }

    quicRes = msQuic->ConnectionStart(connection, msQuicInstance.GetClientConfig(), addrFamily, remoteAddr.to_string().c_str(), path.quicPort);
    if (QUIC_FAILED(quicRes)) {
        assert(false);
        return;
    }
}

QUIC_STATUS QUIC_API MsQuicConnection::QuicConnectionCallback(
    _In_ HQUIC Connection,
    _In_opt_ void* Context,
    _Inout_ QUIC_CONNECTION_EVENT* Event
)
{
    QUIC_STATUS quicRes = QUIC_STATUS_SUCCESS;
    return quicRes;
}
