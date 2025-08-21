#include "MsQuicListener.h"

const QUIC_BUFFER MsQuicListener::Alpn = {
    .Length = (uint32_t)std::size(AlpnStr),
    .Buffer = (uint8_t*)AlpnStr.data()
};

MsQuicListener::MsQuicListener(
    const boost::asio::ip::address& listenAddress,
    const char* certPath,
    const char* keyPath
)
{
    InitConfiguration();
    InitCredentials(certPath, keyPath);

    StartListen(listenAddress);
}

void MsQuicListener::InitConfiguration() {
    QUIC_STATUS quicRes = QUIC_STATUS_SUCCESS;

    QUIC_SETTINGS settings{};
    settings.IsSet.IdleTimeoutMs = true;
    //settings.IdleTimeoutMs = 20'000;
    settings.IdleTimeoutMs = 20'000'000'000; // for debugging and waiting on breakpoints

    settings.IsSet.KeepAliveIntervalMs = true;
    settings.KeepAliveIntervalMs = 10'000;

    settings.IsSet.PeerBidiStreamCount = true;
    settings.PeerBidiStreamCount = 64;

    settings.IsSet.PeerUnidiStreamCount = true;
    settings.PeerUnidiStreamCount = 0;

    // in case datagram needed
    // settings.IsSet.DatagramReceiveEnabled = true;
    // settings.DatagramReceiveEnabled = true;

    auto msQuic = MsQuic::GetInstance().GetMsQuic();
    auto registration = MsQuic::GetInstance().GetRegistration();

    quicRes = msQuic->ConfigurationOpen(
        registration,
        &Alpn, 1,
        &settings, sizeof(settings),
        /*SecurityConfig*/ nullptr,
        &configuration);
    if (QUIC_FAILED(quicRes)) {
        assert(false);
        return;
    }
}

void MsQuicListener::InitCredentials(
    const char* certPath,
    const char* keyPath
)
{
    QUIC_STATUS quicRes = QUIC_STATUS_SUCCESS;

    QUIC_CERTIFICATE_FILE certFile{};

    certFile.CertificateFile = certPath;
    certFile.PrivateKeyFile = keyPath;

    QUIC_CREDENTIAL_CONFIG cred{};
    cred.Type = QUIC_CREDENTIAL_TYPE_CERTIFICATE_FILE;
    cred.CertificateFile = &certFile;

    auto msQuic = MsQuic::GetInstance().GetMsQuic();

    quicRes = msQuic->ConfigurationLoadCredential(configuration, &cred);
    if (QUIC_FAILED(quicRes)) {
        assert(false);
        return;
    }
}

void MsQuicListener::StartListen(const boost::asio::ip::address& listenAddress) {
    QUIC_STATUS quicRes = QUIC_STATUS_SUCCESS;
    auto msQuic = MsQuic::GetInstance().GetMsQuic();
    auto registration = MsQuic::GetInstance().GetRegistration();

    quicRes = msQuic->ListenerOpen(registration, &MsQuicListener::ListenerCallbackStatic, this, &listener);
    if (QUIC_FAILED(quicRes)) {
        assert(false);
        return;
    }

    auto addr = ToQuicAddr(listenAddress);

    quicRes = msQuic->ListenerStart(listener, &Alpn, 1, &addr);
    if (QUIC_FAILED(quicRes)) {
        assert(false);
        return;
    }
}

QUIC_STATUS MsQuicListener::ListenerCallback(
    HQUIC listener_,
    QUIC_LISTENER_EVENT* event
)
{
    QUIC_STATUS quicRes = QUIC_STATUS_SUCCESS;

    switch (event->Type) {
    case QUIC_LISTENER_EVENT_NEW_CONNECTION:
        break;
    case QUIC_LISTENER_EVENT_STOP_COMPLETE:
        break;
    default:
        break;
    }

    return quicRes;
}

QUIC_STATUS QUIC_API MsQuicListener::ListenerCallbackStatic(
    HQUIC listener,
    void* context,
    QUIC_LISTENER_EVENT* event
)
{
    auto _this = static_cast<MsQuicListener*>(context);
    return _this->ListenerCallback(listener, event);
}

QUIC_ADDR MsQuicListener::ToQuicAddr(const boost::asio::ip::address& addr) {
    QUIC_ADDR quicAddr{};

    if (addr.is_v4()) {
        const auto b = addr.to_v4().to_bytes();

        if (sizeof(quicAddr.Ipv4.sin_addr) != b.size()) {
            assert(false);
            return {};
        }

        assert(sizeof(quicAddr.Ipv4.sin_addr) == b.size());

        quicAddr.si_family = QUIC_ADDRESS_FAMILY_INET;
        std::memcpy(&quicAddr.Ipv4.sin_addr, b.data(), b.size());
    }
    else {
        const auto v6 = addr.to_v6();
        const auto b = v6.to_bytes();

        if (sizeof(quicAddr.Ipv6.sin6_addr) != b.size()) {
            assert(false);
            return {};
        }

        quicAddr.si_family = QUIC_ADDRESS_FAMILY_INET6;
        std::memcpy(&quicAddr.Ipv6.sin6_addr, b.data(), b.size());

        uint32_t sid = v6.scope_id();
        if (sid != 0) {
            quicAddr.Ipv6.sin6_scope_id = sid;
        }
    }

    return quicAddr;
}
