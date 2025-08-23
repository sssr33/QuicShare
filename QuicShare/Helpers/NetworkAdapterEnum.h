#pragma once
#include "SafeBoostAsioInclude.h"

#include <vector>

class NetworkAdapterEnum {
public:
    static std::vector<boost::asio::ip::udp::endpoint> Enum(boost::asio::io_context& ioContext);
};
