#include "MsQuic.h"

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

void MsQuic::Init() {
    QUIC_STATUS quicRes = QUIC_STATUS_SUCCESS;

    quicRes = MsQuicOpen2(&msQuic);
    if (QUIC_FAILED(quicRes)) {
        assert(false);
        return;
    }

    InitRegistration();
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
