#include "NetworkAdapterEnum.h"

std::vector<boost::asio::ip::udp::endpoint> NetworkAdapterEnum::Enum(boost::asio::io_context& ioContext) {
    std::vector<boost::asio::ip::udp::endpoint> endpoints;

    boost::asio::ip::udp::resolver resolver(ioContext);
    auto it = resolver.resolve(boost::asio::ip::host_name(), "", boost::asio::ip::resolver_base::flags::passive);

    endpoints.reserve(it.size());

    for (const auto& e : it) {
        endpoints.push_back(e.endpoint());
    }

    return endpoints;
}
