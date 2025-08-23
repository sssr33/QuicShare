#include "MsQuicConfigurationFactory.h"

const QUIC_BUFFER MsQuicConfigurationFactory::Alpn = {
    .Length = (uint32_t)std::size(AlpnStr),
    .Buffer = (uint8_t*)AlpnStr.data()
};

const QUIC_BUFFER& MsQuicConfigurationFactory::GetAlpn() {
    return Alpn;
}

std::tuple<QUIC_STATUS, HQUIC> MsQuicConfigurationFactory::CreateServerConfig() {
    QUIC_STATUS quicRes = QUIC_STATUS_SUCCESS;
    auto msQuic = MsQuic::GetInstance().GetMsQuic();
    auto registration = MsQuic::GetInstance().GetRegistration();
    auto settings = GetDefaultQuicSettings();

    HQUIC configuration = {};

    quicRes = msQuic->ConfigurationOpen(
        registration,
        &Alpn, 1,
        &settings, sizeof(settings),
        /*SecurityConfig*/ nullptr,
        &configuration);
    if (QUIC_FAILED(quicRes)) {
        return { quicRes, configuration };
    }

    auto certFile = GetDefaultCertFile();

    QUIC_CREDENTIAL_CONFIG cred{};
    cred.Type = QUIC_CREDENTIAL_TYPE_CERTIFICATE_FILE;
    cred.CertificateFile = &certFile;

    quicRes = msQuic->ConfigurationLoadCredential(configuration, &cred);
    if (QUIC_FAILED(quicRes)) {
        return { quicRes, configuration };
    }

    return { quicRes, configuration };
}

std::tuple<QUIC_STATUS, HQUIC> MsQuicConfigurationFactory::CreateClientConfig() {
    QUIC_STATUS quicRes = QUIC_STATUS_SUCCESS;
    auto msQuic = MsQuic::GetInstance().GetMsQuic();
    auto registration = MsQuic::GetInstance().GetRegistration();
    auto settings = GetDefaultQuicSettings();

    HQUIC configuration = {};

    quicRes = msQuic->ConfigurationOpen(
        registration,
        &Alpn, 1,
        &settings, sizeof(settings),
        /*SecurityConfig*/ nullptr,
        &configuration);
    if (QUIC_FAILED(quicRes)) {
        return { quicRes, configuration };
    }

    // TODO check if client can use certificates or/and do any kind of authorization
    QUIC_CREDENTIAL_CONFIG cred{};
    cred.Type = QUIC_CREDENTIAL_TYPE_NONE;
    cred.Flags = QUIC_CREDENTIAL_FLAG_CLIENT | QUIC_CREDENTIAL_FLAG_NO_CERTIFICATE_VALIDATION;

    quicRes = msQuic->ConfigurationLoadCredential(configuration, &cred);
    if (QUIC_FAILED(quicRes)) {
        return { quicRes, configuration };
    }

    return { quicRes, configuration };
}

QUIC_SETTINGS MsQuicConfigurationFactory::GetDefaultQuicSettings() {
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

    return settings;
}

QUIC_CERTIFICATE_FILE MsQuicConfigurationFactory::GetDefaultCertFile() {
    QUIC_CERTIFICATE_FILE certFile{};

    certFile.CertificateFile = "server.cert";
    certFile.PrivateKeyFile = "server.key";

    return certFile;
}
