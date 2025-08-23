#pragma once
#include "MsQuic.h"

class MsQuicHelpers {
public:
    static QUIC_ADDR ToQuicAddr(const boost::asio::ip::address& addr);
};
