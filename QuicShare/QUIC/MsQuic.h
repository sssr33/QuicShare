#pragma once
#include "SafeBoostAsioInclude.h"

#include <msquic.h>

class MsQuic {
public:
    static MsQuic& GetInstance();

    const QUIC_API_TABLE* GetMsQuic() const;
    HQUIC GetRegistration() const;
    HQUIC GetServerConfig() const;
    HQUIC GetClientConfig() const;

    void Init();

private:
    MsQuic() = default;

    void InitRegistration();
    void InitConfigurations();

    const QUIC_API_TABLE* msQuic{};
    HQUIC registration{};
    HQUIC serverConfiguration{};
    HQUIC clientConfiguration{};
};
