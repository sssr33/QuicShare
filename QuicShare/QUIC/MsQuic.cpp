#include "MsQuic.h"
#include "MsQuicConfigurationFactory.h"

#include <cassert>
#include <string>

MsQuic& MsQuic::GetInstance() {
    static MsQuic instance;
    return instance;
}

const QUIC_API_TABLE* MsQuic::GetMsQuic() const {
    return msQuic;
}

HQUIC MsQuic::GetRegistration() const {
    return registration;
}

HQUIC MsQuic::GetServerConfig() const {
    return serverConfiguration;
}

HQUIC MsQuic::GetClientConfig() const {
    return clientConfiguration;
}

void MsQuic::Init() {
    QUIC_STATUS quicRes = QUIC_STATUS_SUCCESS;

    quicRes = MsQuicOpen2(&msQuic);
    if (QUIC_FAILED(quicRes)) {
        assert(false);
        return;
    }

    InitRegistration();
    InitConfigurations();
}

void MsQuic::InitRegistration() {
    QUIC_STATUS quicRes = QUIC_STATUS_SUCCESS;
    QUIC_REGISTRATION_CONFIG regConf{};

    regConf.AppName = "QuicShare";
    regConf.ExecutionProfile = QUIC_EXECUTION_PROFILE_LOW_LATENCY;

    quicRes = msQuic->RegistrationOpen(&regConf, &registration);
    if (QUIC_FAILED(quicRes)) {
        assert(false);
        return;
    }
}

void MsQuic::InitConfigurations() {
    auto [resServer, confServer] = MsQuicConfigurationFactory::CreateServerConfig();
    if (QUIC_FAILED(resServer)) {
        assert(false);
        return;
    }

    serverConfiguration = confServer;

    auto [resClient, confClient] = MsQuicConfigurationFactory::CreateClientConfig();
    if (QUIC_FAILED(resClient)) {
        assert(false);
        return;
    }

    clientConfiguration = confClient;
}
