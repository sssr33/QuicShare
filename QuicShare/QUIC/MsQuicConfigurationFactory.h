#pragma once
#include "MsQuic.h"

#include <tuple>

class MsQuicConfigurationFactory {
public:
    static const QUIC_BUFFER& GetAlpn();

    static std::tuple<QUIC_STATUS, HQUIC> CreateServerConfig();
    static std::tuple<QUIC_STATUS, HQUIC> CreateClientConfig();

private:
    static QUIC_SETTINGS GetDefaultQuicSettings();
    static QUIC_CERTIFICATE_FILE GetDefaultCertFile();

    static constexpr const std::string_view AlpnStr = "qs/1";
    static const QUIC_BUFFER Alpn;
};
