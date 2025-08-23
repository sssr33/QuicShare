#include "MsQuicListener.h"
#include "MsQuicConfigurationFactory.h"
#include "MsQuicHelpers.h"

MsQuicListener::MsQuicListener(
    const boost::asio::ip::udp::endpoint& listenEndpoint_
)
    : listenEndpoint(listenEndpoint_)
{
    StartListen(listenEndpoint.address());
}

const boost::asio::ip::udp::endpoint& MsQuicListener::GetListenEndpoint() const {
    return listenEndpoint;
}

uint16_t MsQuicListener::GetListenPort() const {
    return listenPort;
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

    auto addr = MsQuicHelpers::ToQuicAddr(listenAddress);

    quicRes = msQuic->ListenerStart(listener, &MsQuicConfigurationFactory::GetAlpn(), 1, &addr);
    if (QUIC_FAILED(quicRes)) {
        assert(false);
        return;
    }

    QUIC_ADDR listenAddr = {};
    uint32_t bufLength = static_cast<uint32_t>(sizeof(listenAddr));

    quicRes = msQuic->GetParam(listener, QUIC_PARAM_LISTENER_LOCAL_ADDRESS, &bufLength, &listenAddr);
    if (QUIC_FAILED(quicRes)) {
        assert(false);
        return;
    }

    listenPort = QuicAddrGetPort(&listenAddr);
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
