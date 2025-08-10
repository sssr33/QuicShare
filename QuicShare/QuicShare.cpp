#include "QuicShare.h"
#include "Log/Log.h"

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

    localNetworkDiscovery = std::make_unique<LocalNetworkDiscovery>(ioContext, localId);
}

QuicShare::~QuicShare()
{}

void QuicShare::OnStartClicked() {
    LOG_INFO("start clicked");

    localNetworkDiscovery->Announce();
}

void QuicShare::IoContextThreadMain() {
    auto work_guard = boost::asio::make_work_guard(ioContext);
    ioContext.run();
}
