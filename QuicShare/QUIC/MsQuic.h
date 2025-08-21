#pragma once
#include "SafeBoostAsioInclude.h"

#include <msquic.h>

class MsQuic {
public:
    static MsQuic& GetInstance();

    const QUIC_API_TABLE* GetMsQuic() const;
    HQUIC GetRegistration() const;

    void Init();

private:
    MsQuic() = default;

    void InitRegistration();

    const QUIC_API_TABLE* msQuic = nullptr;
    HQUIC registration = nullptr;
};
