#include "QuicShare.h"
#include "Log/Log.h"
#include "QUIC/MsQuic.h"
#include "Helpers/NetworkAdapterEnum.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

QuicShare::QuicShare(QWidget *parent)
    : QMainWindow(parent)
    , ioContextThread(&QuicShare::IoContextThreadMain, this)
{
    ui.setupUi(this);

    Log::Init("logs/log.txt", ui.textEditLog);

    LOG_INFO("Welcome to spdlog!");
    LOG_ERROR("Some error message with arg: {}", 1);

    connect(ui.startButton, &QPushButton::clicked, this, &QuicShare::OnStartClicked);

    {
        boost::uuids::random_generator gen;
        boost::uuids::uuid u = gen();

        localId = boost::uuids::to_string(u);
    }

    MsQuic::GetInstance().Init();

    auto adapterEndpoints = NetworkAdapterEnum::Enum(ioContext);

    StartQuicListeners(adapterEndpoints);
    StartLocalNetworkDiscovery();
}

QuicShare::~QuicShare() {
    ioContext.stop();
}

void QuicShare::LocalPeerAdded(const LocalNetworkPeerInfo& peer) {
    auto selfOrPeer = peer.self ? "SELF" : "PEER";
    auto& peerId = peer.localId;
    auto pathStr = peer.paths.front().ToString();

    /*if (!peer.self && quicListeners.empty()) {
        auto& path = peer.paths.front();
        auto listener = std::make_unique<MsQuicListener>(path.listenAddress, "server.cert", "server.key");
        quicListeners.push_back(std::move(listener));
    }*/

    LOG_INFO("NEW {} added:\n    (id) {}\n    (path) {}", selfOrPeer, peerId, pathStr);
}

void QuicShare::LocalPeerPathAdded(const LocalNetworkPeerInfo& peer, LocalNetworkPeerPath path) {
    auto selfOrPeer = peer.self ? "SELF" : "peer";
    auto& peerId = peer.localId;
    auto pathStr = path.ToString();

    LOG_INFO("added {} NEW PATH:\n    (id) {}\n    (path) {}", selfOrPeer, peerId, pathStr);
}

void QuicShare::OnStartClicked() {
    LOG_INFO("start clicked");

    localNetworkDiscovery->Announce();
}

void QuicShare::IoContextThreadMain() {
    auto workGuard = boost::asio::make_work_guard(ioContext);
    ioContext.run();
}

void QuicShare::StartQuicListeners(const std::vector<boost::asio::ip::udp::endpoint>& adapterEndpoints) {
    for (auto& i : adapterEndpoints) {
        auto addr = i.address();
        auto str = addr.to_string();
        auto isv4 = addr.is_v4();
        auto isLoopback = addr.is_loopback();

        if (i.address().is_loopback()) {
            continue;
        }

        auto listener = std::make_unique<MsQuicListener>(i, "server.cert", "server.key");
        quicListeners.push_back(std::move(listener));
    }
}

void QuicShare::StartLocalNetworkDiscovery() {
    std::vector<LocalNetworkDiscoveryEndpoint> lndEndpoints;

    for (auto& i : quicListeners) {
        lndEndpoints.push_back(
            LocalNetworkDiscoveryEndpoint {
                .endpoint = i->GetListenEndpoint(),
                .quicPort = i->GetListenPort()
            });
    }

    localNetworkDiscovery = std::make_unique<LocalNetworkDiscovery>(ioContext, localId, lndEndpoints);

    connect(localNetworkDiscovery.get(), &LocalNetworkDiscovery::LocalPeerAdded, this, &QuicShare::LocalPeerAdded);
    connect(localNetworkDiscovery.get(), &LocalNetworkDiscovery::LocalPeerPathAdded, this, &QuicShare::LocalPeerPathAdded);
}
